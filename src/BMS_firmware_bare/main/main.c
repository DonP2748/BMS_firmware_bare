//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	main.cpp										    //
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "config.h"
#include "canbus.h"
#include "bms.h"
#include "systimer.h"
#include "log.h"
#include "flash.h"
#include "rtc.h"
#include "bms.h"
#include "timer.h"
#include "hwi2c.h"
//---------------------------------------//

//---------------GLOBAL------------------//
//---------------------------------------//

//--------------PRIVATE------------------//
//---------------------------------------//


int main(void)
{
	
	//board init
	SysTickInit();
	canbus_init();
	hwi2c_init();
	flash_init();
	//device init
	log_init();
	bms_init_system();
	
	while(1){
		bms_error_handle();
		bms_auto_balancing();
		bms_auto_adjusts_temp();
		bms_process_push_notify();
		canbus_process_receive_data();
		process_timer_events();
	}
}



