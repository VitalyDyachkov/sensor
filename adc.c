#include "adc.h"
/*инициализация АЦП*/
void InitADC(void)
{
	ADC_InitTypeDef InitADC;
	GPIO_InitTypeDef InitPort;
	NVIC_InitTypeDef NVIC_Inits;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	InitPort.GPIO_Mode = GPIO_Mode_AN;
	InitPort.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	InitPort.GPIO_PuPd = GPIO_PuPd_NOPULL;
	InitPort.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&InitPort);

	InitADC.ADC_ContinuousConvMode = DISABLE;
	InitADC.ADC_DataAlign = ADC_DataAlign_Right;
	InitADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO;
	InitADC.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None ;
	InitADC.ADC_Resolution = ADC_Resolution_12b;
	InitADC.ADC_ScanDirection = ADC_ScanDirection_Upward;

	ADC_Init(ADC1,&InitADC);
	
	ADC_ChannelConfig(ADC1,ADC_Channel_4,ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1,ADC_Channel_3,ADC_SampleTime_239_5Cycles);
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	//ADC_ITConfig(ADC1,ADC_IT_EOSEQ,ENABLE);
	//ADC_ITConfig(ADC1,ADC_IT_OVR,ENABLE);
	
	NVIC_Inits.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_Inits.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Inits.NVIC_IRQChannelPriority = 2;
	
	NVIC_Init(&NVIC_Inits);
	ADC_Cmd(ADC1,ENABLE);
	ADC_StartOfConversion(ADC1);

}



