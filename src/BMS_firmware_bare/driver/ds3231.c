//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	ds3231.cpp										    //
// AUTHOR 	: 	DonP 												//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "timer.h"
#include "hwi2c.h"
#include "ds3231.h"
//---------------------------------------//

//---------------GLOBAL------------------//
//---------------------------------------//

//--------------PRIVATE------------------//
uint8_t ds3231_read_byte(uint8_t reg);
void ds3231_write_byte(uint8_t reg, uint8_t data);
//---------------------------------------//

// Convert normal decimal numbers to binary coded decimal
uint8_t DecToBcd(uint8_t val)
{
	return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
uint8_t BcdToDec(uint8_t val)
{
	return ( (val/16*10) + (val%16) );
}

uint8_t ds3231_read_byte(uint8_t reg)
{
	uint8_t data = 0;
	hwi2c_read_byte(DS3231_ADDR,reg,&data);
	return data;
}

void ds3231_write_byte(uint8_t reg, uint8_t data)
{
	hwi2c_write_byte(DS3231_ADDR,reg,data);
}


void DS3231_Init(void)
{
	if (DS3231_GetOSF())
	{
		uint8_t temp = ds3231_read_byte(DS3231_HOUR_REGISTER);

#if (DS3231_HOURFORMAT==0) //24h
		ds3231_write_byte(DS3231_HOUR_REGISTER, (temp & 0b10111111));
#else
		ds3231_write_byte(DS3231_HOUR_REGISTER, (temp | 0b01000000));
#endif
		temp = ds3231_read_byte(DS3231_ALARM1_DAYDATE_REGISTER);
#if (DS3231_ALARM_DAYDATE_FORMAT==0) //date of month
		ds3231_write_byte(DS3231_ALARM1_DAYDATE_REGISTER, (temp & 0b10111111));
#else
		ds3231_write_byte(DS3231_ALARM1_DAYDATE_REGISTER, (temp | 0b01000000));
#endif
		temp = ds3231_read_byte(DS3231_ALARM2_DAYDATE_REGISTER);
#if (DS3231_ALARM_DAYDATE_FORMAT==0) //date of month
		ds3231_write_byte(DS3231_ALARM2_DAYDATE_REGISTER, (temp & 0b10111111));
#else
		ds3231_write_byte(DS3231_ALARM2_DAYDATE_REGISTER, (temp | 0b01000000));
#endif

		DS3231_ClearOSF();
	}
}

void DS3231_GetTime(
	uint8_t *cYear,
	uint8_t *cMonth,
	uint8_t *cDate,
	uint8_t *cDay,
	uint8_t *cHour,
	uint8_t *cMinute,
	uint8_t *cSecond)
{

	uint8_t buff[7] = {0};
	hwi2c_read_sequence(DS3231_ADDR,DS3231_SECOND_REGISTER,buff,7);
	
	*cSecond =  buff[0];
	*cMinute =  buff[1];
	*cHour 	 =  buff[2];
#if (DS3231_HOURFORMAT == 0)
	*cHour &= 0b00111111;
#else
	*cHour &= 0b00011111;
#endif
	*cDay   =  buff[3];
	*cDate  =  buff[4];
	*cMonth = (buff[5] & 0b01111111);
	*cYear  =  buff[6];
}


void DS3231_SetTime(
	uint8_t cYear,
	uint8_t cMonth,
	uint8_t cDate,
	uint8_t cDay,
	uint8_t cHour,
	uint8_t cMinute,
	uint8_t cSecond)
{
	
	uint8_t buff[7] = {
		cSecond,
		cMinute,
		cHour,
		cDay,
		cDate,
		cMonth,
		cYear
	};
	hwi2c_write_sequence(DS3231_ADDR,DS3231_SECOND_REGISTER,buff,7);
}




void DS3231_SetAlarmIE(uint8_t cAlarmName, uint8_t cNewSetting)
{
    uint8_t temp = ds3231_read_byte(DS3231_CONTROL_REGISTER);
    ds3231_write_byte(DS3231_CONTROL_REGISTER, 
        (cNewSetting==ON)? 
        (temp | cAlarmName | 0b00000100):
        (temp & ~cAlarmName));

}


uint8_t DS3231_GetAlarmIE(uint8_t cAlarmName)
{  
    return (ds3231_read_byte(DS3231_CONTROL_REGISTER) & cAlarmName);
}


uint8_t DS3231_GetAlarmFlag(uint8_t cAlarmName)
{
	return (ds3231_read_byte(DS3231_STATUS_REGISTER) & cAlarmName);
}


void DS3231_ClearAlarmFlag(uint8_t cAlarmName)
{
	uint8_t temp = ds3231_read_byte(DS3231_STATUS_REGISTER);
    ds3231_write_byte(DS3231_STATUS_REGISTER, (temp & ~cAlarmName));
}


uint8_t DS3231_GetOSF()
{
	return (ds3231_read_byte(DS3231_STATUS_REGISTER) & 0b10000000);
}


void DS3231_ClearOSF(void)
{
    uint8_t temp = ds3231_read_byte(DS3231_STATUS_REGISTER);
        ds3231_write_byte(DS3231_STATUS_REGISTER, (temp & 0b01111111));

}


void DS3231_Set32kHzOutput(uint8_t cNewSetting)
{
    uint8_t temp = ds3231_read_byte(DS3231_STATUS_REGISTER);
    ds3231_write_byte(DS3231_STATUS_REGISTER, 
        (cNewSetting==ON)? 
        (temp | 0b00001000):
        (temp & 0b11110111));

}


void DS3231_SetOscillatorOnOff(uint8_t cNewSetting, uint8_t cFrequency)
{  
    // read control byte in, but zero out current state of RS2 and RS1.
    uint8_t temp = (ds3231_read_byte(DS3231_CONTROL_REGISTER) & 0b11100111);
    if (cFrequency > 3) 
        cFrequency = 3;
    // shift frequency into bits 3 and 4 and set.
    temp |= (cFrequency << 3);
    ds3231_write_byte(DS3231_CONTROL_REGISTER, 
         (cNewSetting==ON)? //if On
         (temp | 0b10000000): // set ~EOSC to 1, leave INTCN as is.
         (temp & 0b01111011)); // set ~EOSC to 0 and INTCN to zero.

}


void DS3231_SetBBSQWOutput(uint8_t cNewSetting)
{
    uint8_t temp = ds3231_read_byte(DS3231_CONTROL_REGISTER);
    ds3231_write_byte(DS3231_CONTROL_REGISTER, 
        (cNewSetting==ON)? //if On
        (temp | 0b01000000): // turn on BBSQW flag
        (temp & 0b10110111)); // turn off BBSQW flag
}


int32_t DS3231_GetTemperature()
{
	int32_t result;
	uint8_t buff[2]={0};
	hwi2c_read_sequence(DS3231_ADDR,DS3231_MSBTEMP_REGISTER,buff,2);
	result = buff[0] + (buff[1]>>6)/4;
	return (int32_t)result; 
}



#if (DS3231_SINGLE_ACCESS==1)

uint8_t DS3231_GetSecond(void)
{
	return ds3231_read_byte(DS3231_SECOND_REGISTER);
}

uint8_t DS3231_GetMinute()
{
	return ds3231_read_byte(DS3231_MINUTE_REGISTER);
}

uint8_t DS3231_GetHour()
{
	return ds3231_read_byte(DS3231_HOUR_REGISTER);
}

uint8_t DS3231_GetDay()
{
	return ds3231_read_byte(DS3231_DAY_REGISTER);
}

uint8_t DS3231_GetDate()
{
	return ds3231_read_byte(DS3231_DATE_REGISTER);
}

uint8_t DS3231_GetMonth()
{
	return ds3231_read_byte(DS3231_MONTH_REGISTER);
}	

uint8_t DS3231_GetYear(void)
{
	return ds3231_read_byte(DS3231_YEAR_REGISTER);
}

void DS3231_SetSecond(uint8_t cSecond)
{
	ds3231_write_byte(DS3231_SECOND_REGISTER,cSecond);
}

void DS3231_SetMinute(uint8_t cMinute)
{
	ds3231_write_byte(DS3231_MINUTE_REGISTER,cMinute);
}

void DS3231_SetHour(uint8_t cHour)
{
	ds3231_write_byte(DS3231_HOUR_REGISTER,cHour);
}

void DS3231_SetDay(uint8_t cDay)
{
	ds3231_write_byte(DS3231_DAY_REGISTER,cDay);
}

void DS3231_SetDate(uint8_t cDate)
{
	ds3231_write_byte(DS3231_DATE_REGISTER,cDate);
}

void DS3231_SetMonth(uint8_t cMonth)
{
	ds3231_write_byte(DS3231_MONTH_REGISTER,cMonth);
}

void DS3231_SetYear(uint8_t cYear)
{
	ds3231_write_byte(DS3231_YEAR_REGISTER,cYear);
}

#endif
