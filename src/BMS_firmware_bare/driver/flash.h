#ifndef _FLASH_H_
#define _FLASH_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	flash.h 											//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//
//---------------------------------------//

//--------------DECLARE------------------//
void flash_init(void);
void flash_write(uint32_t addr,uint32_t val);
uint32_t flash_read(uint32_t addr);
void flash_erase_page(uint32_t addr);
//---------------------------------------//

#endif//_FLASH_H_
