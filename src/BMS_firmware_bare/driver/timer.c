//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	timer.c											    //
// AUTHOR 	: 	DonP 												//
// DATE   	: 	14/08/2023											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "timer.h"
//---------------------------------------//

//---------------GLOBAL------------------//

//---------------------------------------//

//--------------PRIVATE------------------//

//---------------------------------------//

static __IO uint32_t usTicks;
static __IO uint32_t msTicks;

// SysTick_Handler function will be called every 1 us
void SysTick_Handler()
{
	usTicks++; //OverFlow -> 0
	if((usTicks % 1000) == 0)
	{
		msTicks++;
	}
	
}

void SysTickInit()
{
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// Configure the SysTick timer to overflow every 1 us
	SysTick_Config(SystemCoreClock / 1000000);
	// Anti Trap Process if others irq have higher priority and wait for systick count
	__NVIC_SetPriority(SysTick_IRQn,0);
}

void DelayUs(uint32_t us)
{
	// Reload us value
	usTicks = us;
	// Wait until usTick reach zero
	while ((usTicks- us) < us);
}

void DelayMs(uint32_t ms)
{
	// Wait until ms reach zero
	while (ms--)
	{
		// Delay 1ms
		DelayUs(1000);
	}
}

uint32_t GetUsTickCount()
{
	return usTicks;
}
uint32_t GetMsTickCount()
{
	return msTicks;
}

uint32_t Get_Random_Numb(void)
{
	uint32_t TimerCount;
	TimerCount = GetUsTickCount();
	switch (TimerCount & 0x03)
	{
		case 0:  return (TimerCount); 
		case 1:  return (~TimerCount); 
		case 2:  return ((TimerCount >> 1)^(TimerCount   )); 
		default: return ((TimerCount     )^(TimerCount+3 )); 
	}
}
