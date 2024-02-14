#ifndef _DS3231_H_
#define _DS3231_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	app.h 												//
// AUTHOR 	: 	DonP 												//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//
#define DS3231_SINGLE_ACCESS 		1
#define DS3231_HOURFORMAT 			0
#define DS3231_ALARM_DAYDATE_FORMAT 1

#define DS3231_OSCOUT_1Hz 			0
#define DS3231_OSCOUT_1024Hz 		1
#define DS3231_OSCOUT_4096Hz 		2
#define DS3231_OSCOUT_8192Hz 		3

#ifndef OFF
#define OFF 0
#endif
#ifndef ON
#define ON 1
#endif

#define DS3231_SECOND_REGISTER 0x00
#define DS3231_MINUTE_REGISTER 0x01
#define DS3231_HOUR_REGISTER 0x02
#define DS3231_DAY_REGISTER 0x03
#define DS3231_DATE_REGISTER 0x04
#define DS3231_MONTH_REGISTER 0x05
#define DS3231_YEAR_REGISTER 0x06
#define DS3231_ALARM1_SECOND_REGISTER 0x07
#define DS3231_ALARM1_MINUTE_REGISTER 0x08
#define DS3231_ALARM1_HOUR_REGISTER 0x09
#define DS3231_ALARM1_DAYDATE_REGISTER 0x0A
#define DS3231_ALARM2_MINUTE_REGISTER 0x0B
#define DS3231_ALARM2_HOUR_REGISTER 0x0C
#define DS3231_ALARM2_DAYDATE_REGISTER 0x0D
#define DS3231_CONTROL_REGISTER 0x0E
#define DS3231_STATUS_REGISTER 0x0F
#define DS3231_AGINGOFFSET_REGISTER 0x10
#define DS3231_MSBTEMP_REGISTER 0x11
#define DS3231_LSBTEMP_REGISTER 0x12

//---------------------------------------//

//--------------DECLARE------------------//
//---------------------------------------//
void DS3231_Init(void);

void DS3231_GetTime(
	uint8_t *cYear,
	uint8_t *cMonth,
	uint8_t *cDate,
	uint8_t *cDay,
	uint8_t *cHour,
	uint8_t *cMinute,
	uint8_t *cSecond);

void DS3231_SetTime(
	uint8_t cYear,
	uint8_t cMonth,
	uint8_t cDate,
	uint8_t cDay,
	uint8_t cHour,
	uint8_t cMinute,
	uint8_t cSecond);

void DS3231_SetAlarmIE(uint8_t cAlarmName, uint8_t cNewSetting);
uint8_t DS3231_GetAlarmIE(uint8_t cAlarmName);
uint8_t DS3231_GetAlarmFlag(uint8_t cAlarmName);
void DS3231_ClearAlarmFlag(uint8_t cAlarmName);
uint8_t DS3231_GetOSF(void);
void DS3231_ClearOSF(void);
void DS3231_Set32kHzOutput(uint8_t cNewSetting);
void DS3231_SetOscillatorOnOff(uint8_t cNewSetting, uint8_t cFrequency);
void DS3231_SetBBSQWOutput(uint8_t cNewSetting);

int32_t DS3231_GetTemperature(void);
/* Utils */
uint8_t DecToBcd(uint8_t val);
uint8_t BcdToDec(uint8_t val);

#if (DS3231_SINGLE_ACCESS==1)

uint8_t DS3231_GetSecond(void); //0-59
uint8_t DS3231_GetMinute(void); //0-59
uint8_t DS3231_GetHour(void); //0-24
uint8_t DS3231_GetDay(void); //0-7
uint8_t DS3231_GetDate(void); //0-31
uint8_t DS3231_GetMonth(void); //0-12
uint8_t DS3231_GetYear(void); //0-99

void DS3231_SetSecond(uint8_t cSecond); // Also resets Oscillator Stop Flag which is set whenever power is interrupted.
void DS3231_SetMinute(uint8_t cMinute); //0-59
void DS3231_SetHour(uint8_t cHour); //0-24
void DS3231_SetDay(uint8_t cDay); //0-7
void DS3231_SetDate(uint8_t cDate); //0-31
void DS3231_SetMonth(uint8_t cMonth); //0-12
void DS3231_SetYear(uint8_t cYear); //0-99
#endif

#endif //_DS3231_H_
