#ifndef _SYSTIMER_H_
#define _SYSTIMER_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	systimer.h 											//
// AUTHOR 	: 	DonP 												//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//

//---------------------------------------//

//--------------DECLARE------------------//
void process_timer_events(void);
void start_timer(uint32_t millis, void(*callback)(void* data), void* ptr);
void cancel_timer(void(*callback)(void* data));
bool is_timer_running(void(*callback)(void* data));
//---------------------------------------//
#endif//_SYSTIMER_H_