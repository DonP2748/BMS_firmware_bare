#ifndef _TIMER_H_
#define _TIMER_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	timer.h  											//
// AUTHOR 	: 	DonP 												//
// DATE   	: 	14/08/2023											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//

//---------------------------------------//

//--------------DECLARE------------------//
void SysTickInit(void); //Must Init in main
void SysTick_Handler(void);
void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);
uint32_t GetUsTickCount(void);
uint32_t GetMsTickCount(void);
uint32_t Get_Random_Numb(void);
//---------------------------------------//
#endif//_TIMER_H_
