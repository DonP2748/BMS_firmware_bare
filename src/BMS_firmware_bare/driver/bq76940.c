//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	bq76940.c										    //
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	14/08/2023											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "timer.h"
#include "hwi2c.h"
#include "crc8.h"
#include "bq76940.h"
#include <math.h>
//---------------------------------------//

//---------------GLOBAL------------------//
int adc_offset = 0;
int adc_gain = 0;

//---------------------------------------//

//--------------PRIVATE------------------//
static uint8_t alert_flag = false;
static uint8_t Voltage2ADC(uint16_t vol);
static uint8_t bq76940_read_byte(uint8_t reg);
static uint16_t bq76940_read_word(uint8_t reg);
static void bq76940_write_byte(uint8_t reg,uint8_t val);
static void bq76940_alert_extint_init(void);
//---------------------------------------//

void bq76940_init(void)
{
	//init alert pin 
	bq76940_alert_extint_init();
	//enable ADC
	bq76940_write_byte(REG_SYS_CTRL1,ADC_EN);
	//enable CC
	bq76940_write_byte(REG_SYS_CTRL2,CC_EN);
	//follow datasheet CC_CFG
	bq76940_write_byte(REG_CC_CFG,0x19);
	//read gain and offset
	adc_gain = bq76940_read_adc_gain();
	adc_offset = bq76940_read_adc_offset();
	//set OV and UV trip
	bq76940_set_ov_trip(OV_THRESHOLD);
	bq76940_set_ov_trip(UV_THRESHOLD);
	
	//set OCD and SCD
	bq76940_write_byte(REG_PROTECT1,0x0B);
	bq76940_write_byte(REG_PROTECT2,0x9C);
	bq76940_write_byte(REG_PROTECT3,0x50);
	//check device ready, 0 is ready, 1 is not
	uint8_t data = bq76940_read_byte(REG_SYS_STAT);
	if(data & DEVICE_XREADY){
		//clear and wait few second
		bq76940_write_byte(REG_SYS_STAT,data & DEVICE_XREADY);
		DelayMs(5000);
	}
}

int bq76940_read_adc_gain(void)
{
	uint8_t reg1 = bq76940_read_byte(REG_ADCGAIN1);
	uint8_t reg2 = bq76940_read_byte(REG_ADCGAIN2);

	reg1 &= 0x0C;
	reg2 &= 0xE0;
	return (365 + ((reg1 << 1)|(reg2 >> 5)));
}

int  bq76940_read_adc_offset(void)
{
	return bq76940_read_byte(REG_ADCOFFSET);
}


uint16_t bq76940_read_pack_voltage(void)
{
	uint16_t raw = bq76940_read_word(REG_BAT_HI_BYTE);
	//equal 9 page 21 datasheet bq769x0
	return (((4*adc_gain*(raw))/1000)+(NUMBER_OF_CELLS*adc_offset)); 
}

uint16_t bq76940_read_cell_voltage(uint8_t reg)
{
	uint16_t raw = bq76940_read_word(reg);
	return (((raw*adc_gain)/1000)+adc_offset);
}

void bq76940_read_all_cell_voltage(uint16_t *buff, uint8_t len)
{
	int index;
	if(len == NUMBER_OF_CELLS){
		for(index = 0;index < NUMBER_OF_CELLS;index++){
			buff[index] = bq76940_read_cell_voltage(REG_VC1_HI_BYTE + index*2);
		}
	}
}

int16_t bq76940_read_temperature(void)
{
	int32_t temp  = 0;
	int32_t temp_sum = 0;
	int Vtsx = 0;
    int Rts = 0;
	
	uint8_t data = bq76940_read_byte(REG_SYS_CTRL1);
	//check TEMP_SEL for external thermistor (1) or internal die temperature (0)
	switch(data & TEMP_SEL){
		case 0:
			//die temperature store at TS1
			temp = bq76940_read_word(REG_TS1_HI_BYTE);
			//cal Vtsx and TEMPdie, equal 6 7 8 datasheet page 21
			Vtsx = (temp*382)/1000;
			return (25000-((Vtsx-1200)*238))/10000; //celsius
		case 1:
			//cal Vts and Rts, equal 4 and 5 in datasheet page 20
			//TS1
			temp = bq76940_read_word(REG_TS1_HI_BYTE);
			Vtsx = (temp*382)/1000;			// mV
			Rts = (10000*Vtsx)/(3300-Vtsx); // V/A
			//K = 1.0 / (log(Rt / R25) / B + 1.0/T25);
			//T = K - 273.15 ;
			//R25 is R value at 25 Celsius, 103AT-11 have value 10K Ohms at 25 
			//B is Thermistor constant
			temp = 1 / (1 / (273 + 25) +  log(Rts / 10000)/THERMISTOR_CONST ); // K
			temp_sum += temp;
			//TS2
			temp = bq76940_read_word(REG_TS2_HI_BYTE);
			Vtsx = (temp*382)/1000;			// mV
			Rts = (10000*Vtsx)/(3300-Vtsx); // V/A
			temp = 1 / (1 / (273 + 25) +  log(Rts / 10000)/THERMISTOR_CONST ); // K
			temp_sum += temp;		
			//TS3
			temp = bq76940_read_word(REG_TS3_HI_BYTE);
			Vtsx = (temp*382)/1000;			// mV
			Rts = (10000*Vtsx)/(3300-Vtsx); // V/A
			temp = 1 / (1 / (273 + 25) +  log(Rts / 10000)/THERMISTOR_CONST ); // K
			temp_sum += temp;		
			return (temp_sum/3 - 273); //temp average in Celsius
		default:
			break;
	}
	return 0;
}

int16_t bq76940_read_current(void)
{	
	uint16_t cur = 0;
	uint8_t data = bq76940_read_byte(REG_SYS_STAT);
	//check if coulomb counter available
	if(data & CC_READY){
		uint16_t raw = bq76940_read_word(REG_CC_HI_BYTE);
		//equal 3 page 20, 16-bit 2’s Complement Value
		cur = ((int32_t)raw*(844/R_SHUNT))/100;
		//clear cc ready
		data &= ~CC_READY;
		bq76940_write_byte(REG_SYS_STAT,data);
	}
	return (int16_t)cur;
}


void bq76940_cell_balancing(uint8_t numb)
{
	uint8_t index = numb-1; //cell 0-14 
	uint8_t reg = index/5; //register 0-2
	//convert index to bit mask
	uint8_t cmd = bq76940_read_byte(REG_CELLBAL1 + reg);
	cmd |= (uint8_t)1<<(index%5);
	bq76940_write_byte(REG_CELLBAL1 + reg,cmd);
}

void bq76940_clear_balance_reg(void)
{
	for(uint8_t i = 0;i < (NUMBER_OF_CELLS/5);i++){
		bq76940_write_byte(REG_CELLBAL1 + i,0x00);
	}
}

void bq76940_set_charger(uint8_t val)
{
	uint8_t data = bq76940_read_byte(REG_SYS_CTRL2);
	data = (val) ? (data|CHG_ON) : (data&(~CHG_ON));
	bq76940_write_byte(REG_SYS_CTRL2,data);
}

uint8_t bq76940_get_charger(void)
{
	return (bq76940_read_byte(REG_SYS_CTRL2) & CHG_ON);
}

void bq76940_set_discharger(uint8_t val)
{
	uint8_t data = bq76940_read_byte(REG_SYS_CTRL2);
	data = (val) ? (data|DSG_ON) : (data&(~DSG_ON));
	bq76940_write_byte(REG_SYS_CTRL2,data);
}

uint8_t bq76940_get_discharger(void)
{
	return (bq76940_read_byte(REG_SYS_CTRL2) & DSG_ON);
}

void bq76940_enter_ship_mode(void)
{
	// clear SHUT_A and SHUT_B
	bq76940_write_byte(REG_SYS_CTRL1,0x00);
	// SHUT_A=0, SHUT_B=1
	bq76940_write_byte(REG_SYS_CTRL1,0x01);
	// SHUT_A=1, SHUT_B=0
	bq76940_write_byte(REG_SYS_CTRL1,0x02);
}

uint8_t bq76940_error_check(void)
{
	uint8_t data = bq76940_read_byte(REG_SYS_STAT);
	//Device not ready,override alert,under voltage,
	//over voltage,short circuit,over current 
	return data&(DEVICE_XREADY|OVRD_ALERT|UV|OV|SCD|OCD);
}


void bq76940_set_ov_trip(uint16_t val)
{
	bq76940_write_byte(REG_OV_TRIP,Voltage2ADC(val));
}

void bq76940_set_uv_trip(uint16_t val)
{
	bq76940_write_byte(REG_UV_TRIP,Voltage2ADC(val));
}

static uint8_t Voltage2ADC(uint16_t vol)
{
	//page 22
	return (uint8_t)(((vol-adc_offset)*1000/adc_gain + 1)>>4);
}


static uint16_t bq76940_read_word(uint8_t reg)
{
	uint16_t data = (uint16_t)(bq76940_read_byte(reg))<<8|\
					(uint16_t)(bq76940_read_byte(reg + 1));
	return data;
}


static uint8_t bq76940_read_byte(uint8_t reg)
{
#if BQ76940_CRC
	uint8_t count = 3;
	uint8_t buff[4] = {0};
	buff[0] = BQ76940_ADDR;
	buff[1] = reg;
	do{
		hwi2c_read_sequence(BQ76940_ADDR,reg,buff+2,2);
	
	}while((crc8(buff,3) != buff[3])||(count--)); //check crc8
	return buff[2];
#else
	uint8_t data = 0;
	hwi2c_read_byte(BQ76940_ADDR,reg,&data);
	return data;
#endif
}
static void bq76940_write_byte(uint8_t reg,uint8_t val)
{
#if BQ76940_CRC
	uint8_t buff[4] = {0};
	buff[0] = BQ76940_ADDR;
	buff[1] = reg;
	buff[2] = val;
	buff[3] = crc8(buff,3); //create crc8
	hwi2c_write_sequence(BQ76940_ADDR,reg,buff+2,2);
#else
	uint8_t data = val;
	hwi2c_write_byte(BQ76940_ADDR,reg,data);
#endif
}

static void bq76940_alert_extint_init(void)
{
	// Initalization struct
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// Initialize PB12 as input with pull register
	RCC_APB2PeriphClockCmd(BQ76940_ALERT_APB2, ENABLE);
	GPIO_InitStruct.GPIO_Pin = BQ76940_ALERT_PIN;
	GPIO_InitStruct.GPIO_Mode = BQ76940_ALERT_MODE;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BQ76940_ALERT_PORT, &GPIO_InitStruct);
	
	// Initialize PB12 to EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(BQ76940_ALERT_PORT_SRC, BQ76940_ALERT_PIN_SRC);
	
	// Initialize EXTI line 12 for PB12
	EXTI_InitStruct.EXTI_Line = BQ76940_ALERT_EXT_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = BQ76940_ALERT_EXT_EDGE;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// Initialize NVIC for EXTI15_10 IRQ channel
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

uint8_t bq76940_alert_get_flag(void)
{
	return alert_flag;
}	

void bq76940_alert_clear_flag(void)
{
	alert_flag = false;
	bq76940_write_byte(REG_SYS_STAT,DEVICE_XREADY|OVRD_ALERT|UV|OV|SCD|OCD);
}	

void EXTI15_10_IRQHandler(void)
{
	alert_flag = true;
	return;
}