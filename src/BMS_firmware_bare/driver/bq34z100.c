//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	bq34z100.cpp									    //
// AUTHOR 	: 	DonP 												//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "timer.h"
#include "hwi2c.h"
#include "bq34z100.h"
//---------------------------------------//

//---------------GLOBAL------------------//
//---------------------------------------//

//--------------PRIVATE------------------//
static uint16_t bq34z100_read_word(uint8_t cmd);
static uint8_t bq34z100_read_byte(uint8_t cmd);
static void bq34z100_write_byte(uint8_t cmd,uint8_t data);
//---------------------------------------//

uint16_t bq34z100_get_soc(void)
{
	return bq34z100_read_word(CMD_SOC);
}

uint16_t bq34z100_get_now_capacity(void)
{
	return bq34z100_read_word(CMD_REMAIN_CAP);
}

uint16_t bq34z100_get_full_capacity(void)
{
	return bq34z100_read_word(CMD_FULL_CAP);
}

uint16_t bq34z100_get_voltage(void)
{
	return bq34z100_read_word(CMD_VOLTAGE);
}

uint16_t bq34z100_get_average_current(void)
{
	return bq34z100_read_word(CMD_AVRG_CUR);
}

uint16_t bq34z100_get_temperature(void)
{
	return bq34z100_read_word(CMD_TEMP); //Kelvin
}

uint16_t bq34z100_get_flags(void)
{
	return bq34z100_read_word(CMD_FLAGS);
}

uint16_t bq34z100_get_flagb(void)
{
	return bq34z100_read_word(CMD_FLAGB);
}

uint16_t bq34z100_get_power(void)
{
	return bq34z100_read_word(CMD_AVRG_POW);
}

uint16_t bq34z100_get_soh(void)
{
	return bq34z100_read_word(CMD_AVRG_POW);
}

uint16_t bq34z100_get_design_capacity(void)
{
	return bq34z100_read_word(CMD_DESIGN_CAP);
}

static uint16_t bq34z100_read_word(uint8_t cmd)
{
	uint16_t data = 0;
	data |= (bq34z100_read_byte(cmd)<<8);
	data |= bq34z100_read_byte(cmd + 1);
	return data;
}


static uint8_t bq34z100_read_byte(uint8_t cmd)
{
	uint8_t data = 0;
	hwi2c_read_byte(BQ34Z100_ADDR,cmd,&data);
	return data;
}

static void bq34z100_write_byte(uint8_t cmd,uint8_t data)
{
	hwi2c_write_byte(BQ34Z100_ADDR,cmd,data);
}