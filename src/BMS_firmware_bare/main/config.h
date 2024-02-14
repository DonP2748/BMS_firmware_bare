#ifndef _CONFIG_H_
#define _CONFIG_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	config.h 											//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	20/08/2023											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "stm32f10x.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
//---------------------------------------//

//---------------MACRO-------------------//
/*I2C CONFIG*/
#define I2Cx_RCC			RCC_APB1Periph_I2C2
#define I2Cx				I2C2
#define I2C_GPIO_RCC		RCC_APB2Periph_GPIOB
#define I2C_GPIO			GPIOB
#define I2C_PIN_SDA			GPIO_Pin_11
#define I2C_PIN_SCL			GPIO_Pin_10
#define I2C_FREQ			300000

/*CHARGER CONFIG*/
#define BQ76940_ADDR 		0x18
#define BQ76940_CRC 		1

#define NUMBER_OF_CELLS		15		// in Stack
#define OV_THRESHOLD		4200	// in mV
#define UV_THRESHOLD		2700	// in mV
#define R_SHUNT				3		// in hµV/A 
#define V_BALANCE			4000	// Cell charging final voltage in mV
#define V_BALANCE_OFFSET	30		// Cell overcharge voltage in mV to compensate for self-discharge in mV 
#define MAX_TEMP			65		// in °C

#define THERMISTOR_CONST 	3435 	//103AT-11 constant
//BQ76940 ALERT PIN external interrupt
#define BQ76940_ALERT_PIN 		GPIO_Pin_12
#define BQ76940_ALERT_PORT 		GPIOB
#define BQ76940_ALERT_APB2 		RCC_APB2Periph_GPIOB
#define BQ76940_ALERT_MODE 		GPIO_Mode_IPD 			//input pull-down because ALERT active high
#define BQ76940_ALERT_PIN_SRC 	GPIO_PinSource12
#define BQ76940_ALERT_PORT_SRC	GPIO_PortSourceGPIOB
#define BQ76940_ALERT_EXT_LINE 	EXTI_Line12
#define BQ76940_ALERT_EXT_EDGE 	EXTI_Trigger_Rising 	//low to high

/*FUEL GAUSS CONFIG*/
#define BQ34Z100_ADDR 		0x55 		 			

/*RTC CONFIG*/
#define DS3231_ADDR 		0x68

/* Balancing Algorithm */
#define DELTA_1 			100 //mV
#define DELTA_2 			250 //mV

#define BALANCE_TIMEOUT 	(5*60000) // example 5'

/* CAN NETWORK CONGIH */
#define CANx 				CAN1
#define CAN_DATA_SZ 		8 		//0-8 bytes data

#define CAN_BMS_ID 			0x0D 	//11bit msgid = 4bit bmsid + 7 bit cmdid
#define CAN_BMS_ID_MSK 		0x0780
#define CAN_CMD_ID_MSK 		0x7F
/*HANDLE*/
typedef size_t handle_t;

/*FLAGS*/
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wnewline-eof"
#pragma GCC diagnostic ignored "-Wempty-translation-unit"
//#pragma GCC diagnostic ignored "-Wimplicit-int-conversion"
//#pragma GCC diagnostic ignored "-Wmissing-variable-declarations"

//---------------------------------------//

//--------------DECLARE------------------//
//---------------------------------------//
#endif//_CONFIG_H_