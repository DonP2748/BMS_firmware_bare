//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	bms.cpp										    	//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include <math.h>
#include "canbus.h"
#include "rtc.h"
#include "log.h"
#include "timer.h"
#include "ds3231.h"
#include "systimer.h"
#include "bq76940.h"
#include "bq34z100.h"
#include "bms.h"
//---------------------------------------//

//---------------GLOBAL------------------//
#define RECOVERY_TIMEOUT 	60000
//---------------------------------------//

//--------------PRIVATE------------------//
static bms_t _bms = {0};
static bms_t *bms = &_bms;
static bool balance_lck = false;
static bool notify_lck = false;
static void bms_cmd_log_handle(void);
static void bms_cmd_state_handle(uint8_t data);
static void bms_update_info(bms_t *bms);
static bool group_check_cell_valid(uint8_t group, uint8_t index);
static void balance_enter_lck(void);
static void balance_release_lck(void *arg);
static void notify_release_lck(void* arg);
static void bms_set_state_battery(void* arg);
//---------------------------------------//


void bms_init_system(void)
{
	DS3231_Init();
	bq76940_init();
}


void bms_error_handle(void)
{
	//handle charger error
	if(bq76940_alert_get_flag()){
		uint8_t error = bq76940_error_check();
		if(error&DEVICE_XREADY){
			DelayMs(5000);
		}
		if(error&OVRD_ALERT){
			//reserved
		}
		if(error&(UV|OV|SCD|OCD)){
			//auto clear by bq76940 protect
			bq76940_set_charger(0);
			bq76940_set_discharger(0);
			//restart working 60s after error trigger
			start_timer(RECOVERY_TIMEOUT,bms_set_state_battery,bms);
		}
		bq76940_alert_clear_flag();
		
		//save error log
		log_err log = {
			.time = rtc_get_time_stamp(),
			.error= error
		};
		log_save_data(&log);
		//notify
		canbus_transmit(BMS_CMD_ERROR,&log,sizeof(log));
	}
	
	//handle fuel gauge error
	//uint16_t fg = bq34z100_get_flags();
	//reserved
	
	//handle bms error
	//reserved
}

void bms_auto_balancing(void)
{
	if(balance_lck || (bms->state == STATE_OFF)){
		return;
	}
	//update bms info
	bms_update_info(bms);
	//charge balancing
	if(bms->state == STATE_CHARGING){
		//wait if any cell charged fully 
		if(bms->vol_max <= V_BALANCE){
			balance_enter_lck();		
			return;
		}
	}
	//do not balance when discharging
	if(bms->state == STATE_DISCHARGING){
		balance_enter_lck();
		return;
	}
	//discharge balacing 
	if(bms->state == STATE_PARKED){
		//balance when parked
		//do balance
	}
	//no two adjacent cells are balanced simultaneously within each set 
	//of the following: • VC1–VC5• VC6–VC10• VC11–VC15
	
	//balancing
	uint16_t theta_1 = bms->vol_max - bms->vol_min;
	uint16_t theta_2[NUMBER_OF_CELLS] = {0};
	if(theta_1 > DELTA_1){
		//start LLB
		bq76940_clear_balance_reg();
		for (int i =0;i<(NUMBER_OF_CELLS/5);i++){
			uint8_t balance_gr = 0;
			for(int j=0;j<5;j++){
				theta_2[i*5+j-1] = abs(bms->vol_cell[i*5+j-1]-bms->vol_pack);
				if(theta_2[i*5+1-1] > DELTA_2){
					if(group_check_cell_valid(balance_gr,j)){
						balance_gr |= 1<<j;
						bq76940_cell_balancing(i*5+j);
					}
				}
			}
		}
		balance_enter_lck();
	}
}

void bms_process_push_notify(void)
{
	if(notify_lck){
		return;
	}
	uint16_t data[4] = {
		bms->vol_pack,
		bms->temp,
		bms->current,
		bms->pow
	};
	canbus_transmit(BMS_CMD_MEASR,data,sizeof(data));
	
	data[0] = bms->soc;
	data[1] = bms->soh;
	canbus_transmit(BMS_CMD_INFO,data,sizeof(data)/2);
	
	notify_lck = true;
	start_timer(3000,notify_release_lck,NULL);
}

void bms_auto_adjusts_temp(void)
{
	//reserved
	//for hardware support air, liquid cooling
}

static void bms_update_info(bms_t *bms)
{
	//state
	if(bq76940_get_charger()){
		bms->state = STATE_CHARGING;
	}
	else if(bq76940_get_discharger()){
		uint16_t discharging = bq34z100_get_flags() & DSG;
		if(discharging){		
			bms->state = STATE_DISCHARGING;
		}
		else
			bms->state = STATE_PARKED;
	}
	else{
		bms->state = STATE_OFF;
	}
	//voltage
	bms->vol_pack = bq76940_read_pack_voltage();
	bq76940_read_all_cell_voltage(bms->vol_cell,NUMBER_OF_CELLS);
	//find min and max voltage cell
	bms->vol_min = V_BALANCE;
	bms->vol_max = 0;
	for(int i = 0;i<NUMBER_OF_CELLS;i++){
		if(bms->vol_cell[i] < bms->vol_min){
			bms->vol_min = bms->vol_cell[i];
		}
		if(bms->vol_cell[i] > bms->vol_max){
			bms->vol_max = bms->vol_cell[i];
		}
	}
	//temperature
	bms->temp = bq34z100_get_temperature();
	//current
	bms->current = bq34z100_get_average_current();
	//power
	bms->pow = bq34z100_get_power();
	//stage of charge
	bms->soc = bq34z100_get_soc();
	//stage of health
	bms->soh = bq34z100_get_soh();
}

void bms_receive_data_callback(uint8_t event,uint16_t data)
{
	switch (event){
		case BMS_CMD_LOG:
			bms_cmd_log_handle();
			break;
		case BMS_CMD_STATE:
			bms_cmd_state_handle((uint8_t)data);
			break;
		default:
			break;
		
	}
}

static void bms_cmd_log_handle(void)
{
	log_err *log = (log_err*)malloc(sizeof(log_err)*10);
	int numb = log_load_data(log,10);
	for(int i=0;i<numb;i++){
		canbus_transmit(BMS_CMD_ERROR,log+i,sizeof(log_err));
	}
	free(log);
}

static void bms_cmd_state_handle(uint8_t data)
{
	bms->state = data;
	bms_set_state_battery(NULL);
}

//index 0-4
static bool group_check_cell_valid(uint8_t group, uint8_t index)
{
	if((index >= 0)&(index <= 5)){
		if((group&(1<<(index+1)))||(group&(1<<(index-1))) == 0)
			return true;
	}		
	return false;
}

static void bms_set_state_battery(void* arg)
{
	if(bms->state == STATE_CHARGING){
		bq76940_set_charger(1);
		bq76940_set_discharger(0);		
	}
	else if((bms->state == STATE_DISCHARGING)||(bms->state == STATE_PARKED)){
		bq76940_set_charger(0);
		bq76940_set_discharger(1);	
	}
	else if(bms->state == STATE_OFF){
		//reserved
	}
}

static void balance_release_lck(void *arg)
{
	balance_lck = false;
}

static void balance_enter_lck(void)
{
	balance_lck = true;
	start_timer(BALANCE_TIMEOUT,balance_release_lck,NULL);
}

static void notify_release_lck(void* arg)
{
	notify_lck = false;
}