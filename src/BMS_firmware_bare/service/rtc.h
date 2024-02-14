#ifndef _RTC_H_
#define _RTC_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	rtc.h 												//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//
typedef struct date_time_t {
	int year;
	int month;
	int day;
	int dow;
	int hour;
	int minute;
	int second;
} date_time_t;
//---------------------------------------//

//--------------DECLARE------------------//
void rtc_get_date_time(date_time_t* arg);
void rtc_set_date_time(date_time_t  arg);
uint32_t rtc_get_time_stamp(void);
//---------------------------------------//

#endif//_RTC_H_
