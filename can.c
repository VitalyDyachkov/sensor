#include "can.h"

uint32_t temp_reg = 0;
extern uint32_t crc_id;
void InitCAN(void)
{

	CAN_InitTypeDef InitsCAN;
	CAN_FilterInitTypeDef FilterCAN;
	GPIO_InitTypeDef InitPort;
	NVIC_InitTypeDef NVIC_Inits;
	/*включим тактирование CAN модуля*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	/*включим тактирование выводов*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/*ремапинг пинов*/
	do {
				SYSCFG->CFGR1 |= ((uint32_t)SYSCFG_CFGR1_PA11_PA12_RMP);
			}while(!(SYSCFG->CFGR1 & SYSCFG_CFGR1_PA11_PA12_RMP));
	//temp_reg = SYSCFG->CFGR1;
	InitPort.GPIO_Mode = GPIO_Mode_AF;
	InitPort.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	InitPort.GPIO_PuPd = GPIO_PuPd_UP;
	InitPort.GPIO_Speed = GPIO_Speed_2MHz;
			
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_4);
			
	GPIO_Init(GPIOA,&InitPort);
	
//				hcan_x->Init.Prescaler = 96;//24 МГц частота CANa
//				hcan_x->Init.SJW = CAN_SJW_1TQ;// max.CAN_SJW_4TQ
//				hcan_x->Init.BS1 = CAN_BS1_16TQ;//  max.CAN_BS1_16TQ
//				hcan_x->Init.BS2 = CAN_BS2_8TQ;//		max.CAN_BS2_8TQ;

	/*инициализация CAN интерфейса*/
	InitsCAN.CAN_ABOM = ENABLE;
	InitsCAN.CAN_AWUM = DISABLE;
//	/*Speed = 10kBps*/
//	InitsCAN.CAN_Prescaler = 80;
//	InitsCAN.CAN_SJW = CAN_SJW_1tq;
//	InitsCAN.CAN_BS1 = CAN_BS1_4tq;
//	InitsCAN.CAN_BS2 = CAN_BS2_5tq;
//	/*Speed = 20kBps*/
//	InitsCAN.CAN_Prescaler = 80;
//	InitsCAN.CAN_SJW = CAN_SJW_1tq;
//	InitsCAN.CAN_BS1 = CAN_BS1_2tq;
//	InitsCAN.CAN_BS2 = CAN_BS2_2tq;

	/*Speed = 125 kBps*/
	InitsCAN.CAN_Prescaler = 8;
	InitsCAN.CAN_SJW = CAN_SJW_1tq;
	InitsCAN.CAN_BS1 = CAN_BS1_4tq;
	InitsCAN.CAN_BS2 = CAN_BS2_3tq;

//	/*Speed = 500 kBps*/
//	InitsCAN.CAN_Prescaler = 4;
//	InitsCAN.CAN_SJW = CAN_SJW_1tq;
//	InitsCAN.CAN_BS1 = CAN_BS1_2tq;
//	InitsCAN.CAN_BS2 = CAN_BS2_1tq;
	
	
	InitsCAN.CAN_Mode = CAN_Mode_Normal;
	InitsCAN.CAN_NART = DISABLE;
	InitsCAN.CAN_RFLM = DISABLE;

	InitsCAN.CAN_TTCM = DISABLE;
	InitsCAN.CAN_TXFP = DISABLE;
				
				
	FilterCAN.CAN_FilterActivation = ENABLE;
	FilterCAN.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	FilterCAN.CAN_FilterIdHigh = 0x0230 << 5;
	FilterCAN.CAN_FilterIdLow = 0x0240<< 5;
	FilterCAN.CAN_FilterMaskIdHigh = 0x0250 << 5;
	FilterCAN.CAN_FilterMaskIdLow = 0x0260 << 5;
//	FilterCAN.CAN_FilterIdHigh = 0x0000;
//	FilterCAN.CAN_FilterIdLow = 0x0000;
//	FilterCAN.CAN_FilterMaskIdHigh = 0x0000;
//	FilterCAN.CAN_FilterMaskIdLow = 0x0000;
	FilterCAN.CAN_FilterMode = CAN_FilterMode_IdList;
	FilterCAN.CAN_FilterNumber =  1;
	FilterCAN.CAN_FilterScale = CAN_FilterScale_16bit;
	

	
	NVIC_Inits.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_Inits.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Inits.NVIC_IRQChannelPriority = 2;
	
	NVIC_Init(&NVIC_Inits);
		CAN_DeInit(CAN);
	CAN_Init(CAN,&InitsCAN);
	CAN_FilterInit(&FilterCAN);
	CAN_ITConfig(CAN,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN,CAN_IT_ERR,ENABLE);
}
void SendDataCAN(uint8_t type_data,uint16_t data)
{
	CanTxMsg msf_first_can;
	uint8_t arr_id[4];
	uint32_t copy_crc_id = crc_id;
	if(type_data == TEMPERATURE)
	{
			//msf_first_can.ExtId = 0x00000777;
			msf_first_can.StdId = 0x00000777;
	}
	else
	{
			//msf_first_can.ExtId = 0x00000555;
			msf_first_can.StdId = 0x00000555;
	}		
	for (int i = 0 ;i < 4;i++)
	{
			arr_id[i] = copy_crc_id;
			copy_crc_id >>= 8;
	}
	
	for(int i = 0;i<4;i++)
	{
		msf_first_can.Data[i] = arr_id[i];
	}
	msf_first_can.Data[4] = data >> 8 ;
	msf_first_can.Data[5] = data;
	
	msf_first_can.DLC = 8;
	
	msf_first_can.IDE = CAN_Id_Standard;
	msf_first_can.RTR = 0;
	
	
	CAN_Transmit(CAN,&msf_first_can);

}
