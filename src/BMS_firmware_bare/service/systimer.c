//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	systimer.c									    	//
// AUTHOR 	: 	DonP 												//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "timer.h"
#include "systimer.h"
//---------------------------------------//

//---------------GLOBAL------------------//
#define TIMER_TYPE_SHORT	    0
#define TIMER_TYPE_LONG		    1
#define MAX_TIMER_COUNT		    24

#define TIMER_EXPIRED(timer) ((uint32_t)(GetMsTickCount() - timer.start) >= timer.time)



//---------------------------------------//

//--------------PRIVATE------------------//
typedef struct timer_t {
	uint32_t start;
	uint32_t time;
	void(*callback)(void* param);
	void* param;
} timer_t;

static timer_t timers[MAX_TIMER_COUNT] = { 0 };
//---------------------------------------//


void process_timer_events(void) 
{
	int index;
	static void* param = NULL;
	static void(*callback)(void* param) = NULL;

	for (index = 0; index < MAX_TIMER_COUNT; index++)
	{
		if ((timers[index].callback != NULL) && TIMER_EXPIRED(timers[index]))
		{
			callback = timers[index].callback;
			param = timers[index].param;
			timers[index].callback = NULL;
			callback(param);
		}
	}
}

void start_timer(uint32_t millis, void(*callback)(void* data), void* ptr)
{
	int index;
	for (index = 0; index < MAX_TIMER_COUNT; index++)
	{
		if ((timers[index].callback == NULL) || (timers[index].callback == callback))
		{
			timers[index].start = GetMsTickCount();
			timers[index].callback = callback;
			timers[index].time = millis;
			timers[index].param = ptr;

			for (index = index + 1; index < MAX_TIMER_COUNT; index++) {
				if (timers[index].callback == callback)
					timers[index].callback = NULL;
			}
			return;
		}
	}
}

void cancel_timer(void(*callback)(void* data)) 
{
	int index;
	for (index = 0; index < MAX_TIMER_COUNT; index++) 
	{
		if (timers[index].callback == callback) 
		{
			timers[index].callback = NULL;
			break;
		}
	}
}

bool is_timer_running(void(*callback)(void* data)) 
{
	int index;
	for (index = 0; index < MAX_TIMER_COUNT; index++) 
	{
		if (timers[index].callback == callback)
			return true;
	}
	return false;
}

