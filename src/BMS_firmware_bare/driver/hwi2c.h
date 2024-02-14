#ifndef _HWI2C_H_
#define _HWI2C_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	hwi2c.h  											//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	20/08/2023											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//

//---------------------------------------//

//--------------DECLARE------------------//
void hwi2c_init(void);
void hwi2c_write_byte(uint8_t address,uint8_t reg, uint8_t data);
void hwi2c_read_byte(uint8_t address,uint8_t reg, uint8_t* data);
void hwi2c_write_sequence(uint8_t address,uint8_t reg, uint8_t* data, uint8_t len);
void hwi2c_read_sequence(uint8_t address,uint8_t reg, uint8_t* data, uint8_t len);
//---------------------------------------//
#endif//_HWI2C_H_