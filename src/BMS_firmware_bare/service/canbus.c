//////////////////////////////////////////////////////////////////////
// All rights reserved. This program and the accompanying materials	//
// are made available under the terms of the Public License v1.0	//
// and Distribution License v1.0 which accompany this distribution.	//
// MODULE  	:	canbus.cpp										    //
// AUTHOR 	: 	DonP - donp172748@gmail.com							//
// DATE   	: 	--/--/----											//
//////////////////////////////////////////////////////////////////////


//--------------INCLUDE------------------//
#include "bms.h"
#include "canbus.h"
//---------------------------------------//

//---------------GLOBAL------------------//
//---------------------------------------//

//--------------PRIVATE------------------//
static CanTxMsg TxPacket = {0};
static CanRxMsg RxPacket = {0};
static CanTxMsg *pTxPacket = &TxPacket;
static CanRxMsg *pRxPacket = &RxPacket;
//---------------------------------------//

void canbus_init(void)
{
	CAN_InitTypeDef CAN_InitStruct = {0};
	//set config default or custom
	CAN_StructInit(&CAN_InitStruct);
	CAN_Init(CANx,&CAN_InitStruct);
	
	//can filter config
    CAN_FilterInitTypeDef canFilterConfig;
    canFilterConfig.CAN_FilterNumber = 0;
    canFilterConfig.CAN_FilterMode = CAN_FilterMode_IdMask;
    canFilterConfig.CAN_FilterScale = CAN_FilterScale_32bit;
    canFilterConfig.CAN_FilterIdHigh = CAN_BMS_ID << (5+7);
    canFilterConfig.CAN_FilterIdLow = 0x0000;
    canFilterConfig.CAN_FilterMaskIdHigh = CAN_BMS_ID << (5+7);
    canFilterConfig.CAN_FilterMaskIdLow = 0x0000;
    canFilterConfig.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    canFilterConfig.CAN_FilterActivation = ENABLE;
	
    CAN_FilterInit(&canFilterConfig);
	
	CAN_SlaveStartBank(0);
}

void canbus_transmit(uint8_t cmd,void* data,uint8_t size)
{
	uint8_t sz = (size < CAN_DATA_SZ) ? size : CAN_DATA_SZ;
	pTxPacket->StdId = ((CAN_BMS_ID<<7)&CAN_BMS_ID_MSK)|(cmd&CAN_CMD_ID_MSK);
	pTxPacket->ExtId = 0;
	pTxPacket->IDE 	 = CAN_Id_Standard;
	pTxPacket->RTR 	 = CAN_RTR_DATA;
	pTxPacket->DLC 	 = sz;
	memcpy(pTxPacket->Data,data,sz);
	CAN_Transmit(CANx,pTxPacket);
}

void canbus_process_receive_data(void)
{
	if(CAN_MessagePending(CANx,CAN_FIFO0)){
		CAN_Receive(CANx,CAN_FIFO0,pRxPacket);
		uint8_t event = (uint8_t)(pTxPacket->StdId & 0x7F);
		uint16_t data = 0;
		memcpy(&data,pRxPacket->Data,sizeof(data));
		bms_receive_data_callback(event,data);
	}
}

