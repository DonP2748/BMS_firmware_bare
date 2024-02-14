#ifndef _BMS_H_
#define _BMS_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	bms.h 												//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//
#define STATE_OFF	 		0
#define STATE_CHARGING		1
#define STATE_DISCHARGING	2
#define STATE_PARKED 		3
//---------------------------------------//

//--------------DECLARE------------------//


typedef enum BMS_CMD_type{
	BMS_CMD_LOG   = 0x6A,
	BMS_CMD_STATE = 0x6B,
	BMS_CMD_MEASR = 0x6C, 
	BMS_CMD_INFO  = 0x6D,
	BMS_CMD_ERROR = 0x6E
}BMS_CMD_type;

typedef struct bms_t{
	//state
	uint16_t state;
	//voltage
	uint16_t vol_pack;
	uint16_t vol_cell[NUMBER_OF_CELLS];
	uint16_t vol_max;
	uint16_t vol_min;
	//temperature
	uint16_t temp;
	//current
	uint16_t current;
	//power
	uint16_t pow;
	//stage of charge
	uint16_t soc;
	//stage of health
	uint16_t soh;
	//error flag
	uint16_t err_flg;
}bms_t;


void bms_init_system(void);
void bms_error_handle(void); 
void bms_auto_balancing(void);
void bms_auto_adjusts_temp(void);
void bms_process_push_notify(void);
void bms_receive_data_callback(uint8_t event,uint16_t data);
//---------------------------------------//

#endif//_BMS_H_
