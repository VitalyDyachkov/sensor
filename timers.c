#include "timers.h"


void InitTIM_1(void)
{
	
	TIM_TimeBaseInitTypeDef InitsTIM;
	NVIC_InitTypeDef NVIC_Inits;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	InitsTIM.TIM_ClockDivision = TIM_CKD_DIV1;
	InitsTIM.TIM_CounterMode = TIM_CounterMode_Up;
	InitsTIM.TIM_Period = 1000;
	InitsTIM.TIM_Prescaler = 1000;
	
	NVIC_Inits.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
	NVIC_Inits.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Inits.NVIC_IRQChannelPriority = 1;
	
	NVIC_Init(&NVIC_Inits);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	TIM_TimeBaseInit(TIM1,&InitsTIM);
  TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);
	TIM_Cmd(TIM1,ENABLE);

}
void InitTIM_3(void)
{
	TIM_TimeBaseInitTypeDef InitsSetTIM;
	TIM_OCInitTypeDef InitsTIM;
	GPIO_InitTypeDef InitPort;
	NVIC_InitTypeDef NVIC_Inits;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	InitPort.GPIO_Mode = GPIO_Mode_AF;
	InitPort.GPIO_Pin = GPIO_Pin_1;
	InitPort.GPIO_PuPd = GPIO_PuPd_NOPULL;
	InitPort.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&InitPort);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_1);
	
	InitsSetTIM.TIM_ClockDivision = TIM_CKD_DIV1;
	InitsSetTIM.TIM_Prescaler = 1;
	InitsSetTIM.TIM_Period = 0x0032;
	
	InitsTIM.TIM_OCPolarity = TIM_OCPolarity_High;
	InitsTIM.TIM_OCMode = TIM_OCMode_Toggle;
	InitsTIM.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable;
	InitsTIM.TIM_Pulse = 0x0032;
	
	NVIC_Inits.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_Inits.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Inits.NVIC_IRQChannelPriority = 2;
	
	NVIC_Init(&NVIC_Inits);
	
	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);
	
	TIM_TimeBaseInit(TIM3,&InitsSetTIM);
	TIM_OC4Init(TIM3,&InitsTIM);
	TIM_Cmd(TIM3,ENABLE);

}

