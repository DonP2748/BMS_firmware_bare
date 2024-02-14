//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	log.cpp											    //
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "flash.h"
#include "log.h"
//---------------------------------------//

//---------------GLOBAL------------------//
#define PAGESZ 					1024
#define LOG_ERR_PAGE_ADDR 		0x800FC00
//---------------------------------------//

//--------------PRIVATE------------------//
static uint16_t datasz = 0;
//---------------------------------------//
void log_init(void)
{
	flash_erase_page(LOG_ERR_PAGE_ADDR);
}

void log_save_data(log_err* log)
{
	if(datasz >= PAGESZ){
		flash_erase_page(LOG_ERR_PAGE_ADDR);
		datasz = 0;
	}
	flash_write(LOG_ERR_PAGE_ADDR + datasz,log->time);
	flash_write(LOG_ERR_PAGE_ADDR + datasz + 4,log->error);
	datasz += sizeof(log_err);
}

int log_load_data(log_err* log,uint8_t len)
{
	uint32_t id = datasz;
	//load len number of nearest error log
	for(int i=0;i<len;i++){
		if(id <= 0){
			return i;
		}
		(log+i)->error= flash_read(LOG_ERR_PAGE_ADDR + id);
		(log+i)->time = flash_read(LOG_ERR_PAGE_ADDR + id-4);
		id -= sizeof(log_err);
	}
	return len;
}

