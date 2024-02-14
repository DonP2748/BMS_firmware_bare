#ifndef _CANBUS_H_
#define _CANBUS_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	canbus.h											//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//
//---------------------------------------//

//--------------DECLARE------------------//
void canbus_init(void);
void canbus_transmit(uint8_t cmd,void* data,uint8_t size);
void canbus_process_receive_data(void);
//---------------------------------------//

#endif//_CANBUS_H_
