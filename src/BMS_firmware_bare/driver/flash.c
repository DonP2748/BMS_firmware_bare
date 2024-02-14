//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	flash.cpp										    //
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "flash.h"
//---------------------------------------//

//---------------GLOBAL------------------//
//---------------------------------------//

//--------------PRIVATE------------------//
//---------------------------------------//
void flash_init(void)
{	
	FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency( FLASH_Latency_2);
	
}

uint32_t flash_read(uint32_t addr)
{
	uint32_t data = *(__IO uint32_t*)addr;
	return data;
}

void flash_write(uint32_t addr,uint32_t val)
{
	FLASH_Unlock();
	FLASH_ProgramWord((uint32_t)addr,val);
	FLASH_Lock();
}

void flash_erase_page(uint32_t addr)
{
	FLASH_Unlock();
	FLASH_ErasePage(addr);	
	FLASH_Lock();
}
