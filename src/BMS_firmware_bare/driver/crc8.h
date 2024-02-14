#ifndef _CRC8_H_
#define _CRC8_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	crc8.h  											//
// AUTHOR 	: 	DonP 												//
// DATE   	: 	14/08/2023											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//

//---------------------------------------//

//--------------DECLARE------------------//
uint8_t crc8(uint8_t *data, int len);
//---------------------------------------//
#endif//_CRC8_H_
