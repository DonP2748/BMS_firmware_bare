#ifndef _BQ34Z100_H_
#define _BQ34Z100_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	bq34z100.h											//
// AUTHOR 	: 	DonP 												//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//
//Control Command
#define CMD_CTRL 				0x00//&0x01
//Control Subcommands

//Normal Commands
#define CMD_SOC 				0x02
#define CMD_MAX_ERR 			0x03
#define CMD_REMAIN_CAP 			0x04//&0x05
#define CMD_FULL_CAP 			0x06//&0x07
#define CMD_VOLTAGE 			0x08//&0x09
#define CMD_AVRG_CUR			0x0A//&0x0B
#define CMD_TEMP				0x0C//&0x0D
#define CMD_FLAGS				0x0E//&0x0F
#define CMD_FLAGB 				0x12//&0x13
#define CMD_CURRENT 			0x10//&0x11
//Extended Data Commands
#define CMD_AVRG_POW 			0x26//&0x27
#define CMD_INTERNAL_TEMP 		0x2A//&0x2B
#define CMD_CYCLE_CNT 			0x2C//&0x2D
#define CMD_SOH 				0x2E//&0x2F
#define CMD_CHG_VOL 			0x30//&0x31
#define CMD_CHG_CUR 			0x32//&0x33
#define CMD_DESIGN_CAP 			0x3C//&0x3D
#define CMD_DATAFLASH_BLK 		0x3F

//Flags mask bit
//High byte
#define OTC				0x80
#define OTD				0x40
#define BATHI			0x20
#define BATLO			0x10
#define CHG_INH		 	0x08
#define XCHG			0x04
#define FC				0x02
#define CHG				0x01	
//Low byte
#define OCVTAKEN 		0x80
#define CF				0x10
#define SOC1			0x04
#define SOCF			0x02
#define DSG 			0x01
//---------------------------------------//

//--------------DECLARE------------------//
/* Monitor */
uint16_t bq34z100_get_soc(void);
uint16_t bq34z100_get_now_capacity(void);
uint16_t bq34z100_get_full_capacity(void);
uint16_t bq34z100_get_voltage(void);
uint16_t bq34z100_get_average_current(void);
uint16_t bq34z100_get_temperature(void);
uint16_t bq34z100_get_flags(void);
uint16_t bq34z100_get_flagb(void);
uint16_t bq34z100_get_power(void);
uint16_t bq34z100_get_soh(void);
uint16_t bq34z100_get_design_capacity(void);
//---------------------------------------//

#endif//_BQ34Z100_H_
