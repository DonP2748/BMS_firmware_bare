//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	hwi2c.c											    //
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	20/08/2023											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"
#include "misc.h"

#include "hwi2c.h"
//---------------------------------------//

//---------------GLOBAL------------------//
#define TIMEOUT             1000
#define I2C_WAIT_EVT(f,e,t) timeout = t;\
                            while ((f(I2Cx,e))||(timeout--)); \
                            timeout = 0
//---------------------------------------//

//--------------PRIVATE------------------//
static void hwi2c_start(void);
static void hwi2c_stop(void);
static void hwi2c_address_direction(uint8_t address, uint8_t direction);
static void hwi2c_transmit(uint8_t byte);
static uint8_t hwi2c_receive_ack(void);
static uint8_t hwi2c_receive_nack(void);
static uint16_t timeout = 0;
//---------------------------------------//


void hwi2c_init(void)
{
	// Initialization struct
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Step 1: Initialize I2C
	RCC_APB1PeriphClockCmd(I2Cx_RCC, ENABLE);
	I2C_InitStruct.I2C_ClockSpeed = I2C_FREQ;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx, &I2C_InitStruct);
	I2C_Cmd(I2Cx, ENABLE);
	
	// Step 2: Initialize GPIO as open drain alternate function
	RCC_APB2PeriphClockCmd(I2C_GPIO_RCC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}

void hwi2c_write_byte(uint8_t address, uint8_t reg, uint8_t data)
{
	hwi2c_start();
	hwi2c_address_direction((uint8_t)(address << 1), I2C_Direction_Transmitter);
    hwi2c_transmit(reg);
	hwi2c_transmit(data);
	hwi2c_stop();
}


void hwi2c_write_sequence(uint8_t address, uint8_t reg, uint8_t* data, uint8_t len)
{
	hwi2c_start();
	hwi2c_address_direction((uint8_t)(address << 1), I2C_Direction_Transmitter);
    hwi2c_transmit(reg);
    for(int i=0;i<len;i++)
    {
        hwi2c_transmit(*(data+i));   
    }
	hwi2c_stop();
}

void hwi2c_read_byte(uint8_t address, uint8_t reg, uint8_t* data)
{
	hwi2c_start();
    hwi2c_address_direction((uint8_t)(address << 1), I2C_Direction_Transmitter);
    hwi2c_transmit(reg);
    hwi2c_start();
	hwi2c_address_direction((uint8_t)((uint8_t)(address << 1)), I2C_Direction_Receiver);
	*data = hwi2c_receive_nack();
	hwi2c_stop();
}

void hwi2c_read_sequence(uint8_t address, uint8_t reg, uint8_t* data, uint8_t len)
{
	hwi2c_start();
    hwi2c_address_direction((uint8_t)(address << 1), I2C_Direction_Transmitter);
    hwi2c_transmit(reg);
    hwi2c_start();
	hwi2c_address_direction((uint8_t)(address << 1), I2C_Direction_Receiver);
    for(int i=0;i<len-1;i++)
    {
        *(data+i) = hwi2c_receive_ack();
    }
    data[len-1] = hwi2c_receive_nack();
	hwi2c_stop();
}

static void hwi2c_start(void)
{
    I2C_WAIT_EVT(I2C_GetFlagStatus,I2C_FLAG_BUSY,TIMEOUT);
	I2C_GenerateSTART(I2Cx, ENABLE);
    I2C_WAIT_EVT(!I2C_CheckEvent,I2C_EVENT_MASTER_MODE_SELECT,TIMEOUT);
}

static void hwi2c_stop(void)
{
	I2C_GenerateSTOP(I2Cx, ENABLE);
    I2C_WAIT_EVT(I2C_GetFlagStatus,I2C_FLAG_STOPF,TIMEOUT);
}

static void hwi2c_address_direction(uint8_t address, uint8_t direction)
{
	I2C_Send7bitAddress(I2Cx, address, direction);
	if (direction == I2C_Direction_Transmitter)
	{
        I2C_WAIT_EVT(!I2C_CheckEvent,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED,TIMEOUT);
	}
	else if (direction == I2C_Direction_Receiver)
	{	
        I2C_WAIT_EVT(!I2C_CheckEvent,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED,TIMEOUT);
	}
}

static void hwi2c_transmit(uint8_t byte)
{
	I2C_SendData(I2Cx, byte);
    I2C_WAIT_EVT(!I2C_CheckEvent,I2C_EVENT_MASTER_BYTE_TRANSMITTED,TIMEOUT);
}

static uint8_t hwi2c_receive_ack(void)
{
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
    I2C_WAIT_EVT(!I2C_CheckEvent,I2C_EVENT_MASTER_BYTE_RECEIVED,TIMEOUT);
	return I2C_ReceiveData(I2Cx);
}

static uint8_t hwi2c_receive_nack(void)
{
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_WAIT_EVT(!I2C_CheckEvent,I2C_EVENT_MASTER_BYTE_RECEIVED,TIMEOUT);
	return I2C_ReceiveData(I2Cx);
}

