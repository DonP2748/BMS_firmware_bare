//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	crc8.c											    //
// AUTHOR 	: 	DonP 												//
// DATE   	: 	14/08/2023											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "crc8.h"
//---------------------------------------//

//---------------GLOBAL------------------//
#define POLY 0x07 // The polynomial x^8 + x^2 + x + 1
#define INIT 0x00 // The initial value is 0
//---------------------------------------//

//--------------PRIVATE------------------//

//---------------------------------------//
uint8_t crc8(uint8_t *data, int len) 
{
	unsigned char crc = INIT;
	for (int i = 0; i < len; i++) {
		crc ^= data[i];
		for (int j = 0; j < 8; j++) {
			if (crc & 0x80) {
				crc = (uint8_t)((crc << 1) ^ POLY);
			} else {
				crc = (uint8_t)(crc << 1);
			}
		}
	}
	return crc;
}