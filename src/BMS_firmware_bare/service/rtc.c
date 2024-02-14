//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	rtc.cpp											    //
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include <time.h>
#include "ds3231.h"
#include "rtc.h"
//---------------------------------------//

//---------------GLOBAL------------------//
//---------------------------------------//

//--------------PRIVATE------------------//
//---------------------------------------//

void rtc_get_date_time(date_time_t* arg)
{
	DS3231_GetTime(
	(uint8_t*)&arg->year,
	(uint8_t*)&arg->month,
	(uint8_t*)&arg->dow,
	(uint8_t*)&arg->day,
	(uint8_t*)&arg->hour,
	(uint8_t*)&arg->minute,
	(uint8_t*)&arg->second
	);
}
void rtc_set_date_time(date_time_t arg)
{
	DS3231_SetTime(
	(uint8_t)arg.year,
	(uint8_t)arg.month,
	(uint8_t)arg.dow,
	(uint8_t)arg.day,
	(uint8_t)arg.hour,
	(uint8_t)arg.minute,
	(uint8_t)arg.second
	);
}
uint32_t rtc_get_time_stamp(void)
{
	date_time_t dt = {0};
	rtc_get_date_time(&dt);
	
	struct tm tm = {
		.tm_year = dt.year - 1900,
		.tm_mon  = dt.month - 1,
		.tm_wday = dt.dow -1,
		.tm_mday = dt.day,
		.tm_hour = dt.hour,
		.tm_min  = dt.minute,
		.tm_sec  = dt.second,
		.tm_isdst= -1	
	};
	time_t t = mktime(&tm);
	if((int)t == -1){
		return 0;
	}
	uint32_t timestamp = (uint32_t)t;
	return timestamp;	
}

