#ifndef _LOG_H_
#define _LOG_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	log.h 												//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//
//---------------------------------------//

//--------------DECLARE------------------//
typedef struct log_err{
	uint32_t time;
	uint32_t error;
}log_err;

void log_init(void);
void log_save_data(log_err* log);
int log_load_data(log_err* log,uint8_t len);
//---------------------------------------//

#endif//_LOG_H_
