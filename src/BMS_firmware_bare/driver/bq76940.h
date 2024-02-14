#ifndef _BQ76940_H_
#define _BQ76940_H_

//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	bq76940.h  											//
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	14/08/2023											//
//////////////////////////////////////////////////////////////////////

//--------------INCLUDE------------------//
#include "config.h"
//---------------------------------------//

//---------------MACRO-------------------//
// register map
#define REG_SYS_STAT  	0x00
#define REG_CELLBAL1  	0x01
#define REG_CELLBAL2  	0x02
#define REG_CELLBAL3  	0x03
#define REG_SYS_CTRL1 	0x04
#define REG_SYS_CTRL2 	0x05
#define REG_PROTECT1  	0x06
#define REG_PROTECT2  	0x07
#define REG_PROTECT3  	0x08
#define REG_OV_TRIP   	0x09
#define REG_UV_TRIP   	0x0A
#define REG_CC_CFG    	0x0B

#define REG_VC1_HI_BYTE  0x0C
#define REG_VC1_LO_BYTE  0x0D
#define REG_VC2_HI_BYTE  0x0E
#define REG_VC2_LO_BYTE  0x0F
#define REG_VC3_HI_BYTE  0x10
#define REG_VC3_LO_BYTE  0x11
#define REG_VC4_HI_BYTE  0x12
#define REG_VC4_LO_BYTE  0x13
#define REG_VC5_HI_BYTE  0x14
#define REG_VC5_LO_BYTE  0x15
#define REG_VC6_HI_BYTE  0x16
#define REG_VC6_LO_BYTE  0x17
#define REG_VC7_HI_BYTE  0x18
#define REG_VC7_LO_BYTE  0x19
#define REG_VC8_HI_BYTE  0x1A
#define REG_VC8_LO_BYTE  0x1B
#define REG_VC9_HI_BYTE  0x1C
#define REG_VC9_LO_BYTE  0x1D
#define REG_VC10_HI_BYTE 0x1E
#define REG_VC10_LO_BYTE 0x1F
#define REG_VC11_HI_BYTE 0x20
#define REG_VC11_LO_BYTE 0x21
#define REG_VC12_HI_BYTE 0x22
#define REG_VC12_LO_BYTE 0x23
#define REG_VC13_HI_BYTE 0x24
#define REG_VC13_LO_BYTE 0x25
#define REG_VC14_HI_BYTE 0x26
#define REG_VC14_LO_BYTE 0x27
#define REG_VC15_HI_BYTE 0x28
#define REG_VC15_LO_BYTE 0x29

#define REG_BAT_HI_BYTE 0x2A
#define REG_BAT_LO_BYTE 0x2B

#define REG_TS1_HI_BYTE 0x2C
#define REG_TS1_LO_BYTE 0x2D
#define REG_TS2_HI_BYTE 0x2E
#define REG_TS2_LO_BYTE 0x2F
#define REG_TS3_HI_BYTE 0x30
#define REG_TS3_LO_BYTE 0x31

#define REG_CC_HI_BYTE 0x32
#define REG_CC_LO_BYTE 0x33

#define REG_ADCGAIN1  0x50
#define REG_ADCOFFSET 0x51
#define REG_ADCGAIN2  0x59

// for bit clear operations of the SYS_STAT register
#define REG_SYS_STAT_CC_READY      (0x80)
#define REG_SYS_STAT_DEVICE_XREADY (0x20)
#define REG_SYS_STAT_OVRD_ALERT    (0x10)
#define REG_SYS_STAT_UV            (0x08)
#define REG_SYS_STAT_OV            (0x04)
#define REG_SYS_STAT_SCD           (0x02)
#define REG_SYS_STAT_OCD           (0x01)
#define REG_SYS_STAT_ERROR_MASK    (0x3F)

#define LOW_BYTE(data)  (uint8_t)(0xFF & data)
#define HIGH_BYTE(data) (uint8_t)(0xFF & (data >> 8))

//SYS_STAT bit masks
#define CC_READY 					0x80
#define DEVICE_XREADY 				0x20
#define OVRD_ALERT 					0x10
#define UV 							0x08
#define OV 							0x04
#define SCD 						0x02
#define OCD 						0x01
//SYS_CTRL1 bit masks
#define LOAD_PRESENT 				0x80
#define ADC_EN 						0x10
#define TEMP_SEL 					0x08
#define SHUT_A 						0x02
#define SHUT_B 						0x01
//SYS_CTRL2 bit masks
#define DELAY_DIS 					0x80
#define CC_EN 						0x40
#define CC_ONESHOT 					0x20
#define DSG_ON 						0x02
#define CHG_ON 						0x01



//---------------------------------------//

//--------------DECLARE------------------//
extern int adc_offset;
extern int adc_gain;

void bq76940_init(void);
int  bq76940_read_adc_gain(void);
int  bq76940_read_adc_offset(void);
/* Measurement/Monitor Subsystem */
uint16_t bq76940_read_pack_voltage(void);
uint16_t bq76940_read_cell_voltage(uint8_t reg);
void bq76940_read_all_cell_voltage(uint16_t *buff, uint8_t len);
int16_t bq76940_read_current(void);
int16_t bq76940_read_temperature(void);
/* Protection Subsystem */
uint8_t bq76940_error_check(void);
void bq76940_set_ov_trip(uint16_t val);
void bq76940_set_uv_trip(uint16_t val);
/* Control Subsystem */
void bq76940_cell_balancing(uint8_t numb);
void bq76940_clear_balance_reg(void);
void bq76940_set_charger(uint8_t val);
uint8_t bq76940_get_charger(void);
void bq76940_set_discharger(uint8_t val);
uint8_t bq76940_get_discharger(void);
/* Device Functional Modes */
void bq76940_enter_ship_mode(void);
/* Alert Pin */
uint8_t bq76940_alert_get_flag(void);
void bq76940_alert_clear_flag(void);
void EXTI15_10_IRQHandler(void);
//---------------------------------------//
#endif//_BQ76940_H_
