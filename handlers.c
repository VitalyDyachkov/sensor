#include "handlers.h"
#include "stdbool.h"
extern uint32_t cnt_response;
extern uint32_t time_response;
bool flag_adc = false;
bool flag_get_temperature = false;
bool flag_get_hum = false;
bool flag_get_can_msg = false; 
uint16_t data_adc_1;
uint16_t data_adc_temp[10];
uint16_t data_adc_2;
uint16_t data_adc_hum[10];
int cnt_temp_sample = 0;
int cnt_hum_sample = 0;
float hum;
float temperature;
				uint32_t temperature_data_10sample;
				uint32_t humidyty_data_10sample;
CanRxMsg CANMessage;
uint16_t buff_humm[10];
uint8_t counter;
void ADC1_IRQHandler (void)
{
	//ADC_StopOfConversion(ADC1);
	
	if(ADC_GetFlagStatus(ADC1, ADC_IT_EOC) == SET)
	{
		if(flag_adc == true)
		{
			flag_adc = false;
			
			temperature_data_10sample += ADC_GetConversionValue(ADC1);
			cnt_temp_sample++;
			if(cnt_temp_sample == 10)
			{
				cnt_temp_sample = 0;
				data_adc_1 = temperature_data_10sample / 10 ;
				temperature_data_10sample = 0;
				//temperature = data_adc_1 * 0.0008;
				flag_get_temperature = true;
			}
		}
		else
		{
//				GPIO_SetBits(GPIOA,GPIO_Pin_6);
			flag_adc = true;
			buff_humm[counter] = ADC_GetConversionValue(ADC1);
			humidyty_data_10sample += buff_humm[counter++];
			if(counter == 10)
			{
				counter = 0;
			}
		//humidyty_data_10sample += ADC_GetConversionValue(ADC1);
			
			cnt_hum_sample++;
			
			if(cnt_hum_sample == 10)
			{
				cnt_hum_sample = 0;

				data_adc_2 = humidyty_data_10sample / 10 ;
				humidyty_data_10sample = 0;
				//hum = data_adc_2 * 0.0008;
				flag_get_hum = true;
//				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
			}
		}	
	}
	if(ADC_GetFlagStatus(ADC1, ADC_IT_EOSEQ) == SET)
	{
		ADC_ClearFlag(ADC1, ADC_IT_EOSEQ);
	}
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR) == SET)
	{
		ADC_ClearFlag(ADC1, ADC_FLAG_OVR);
	}
		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOSMP) == SET)
	{
		ADC_ClearFlag(ADC1, ADC_FLAG_EOSMP);
	}
}
void CEC_CAN_IRQHandler (void)
{
	//uint8_t data = 0;
	 if(CAN_GetITStatus (CAN,CAN_IT_FMP0) == SET)
	 {
		 flag_get_can_msg = true; 
		 CAN_Receive(CAN,CAN_FIFO0,&CANMessage);
		 if(CANMessage.StdId == 0x230)
		 {
			 time_response  = CANMessage.Data[0];
			 //ADC_StartOfConversion(ADC1);
			 cnt_response = 0;
		 }
		 else if (CANMessage.StdId == 0x240)
		 {
			 
		 }
		 //CAN_ClearITPendingBit(CAN,CAN_IT_FF0);
	 }
	 if(CAN_GetITStatus (CAN,CAN_IT_BOF) == SET)
	 {
		 	InitCAN();
	 }
}
void TIM1_BRK_UP_TRG_COM_IRQHandler (void)
{
	if(TIM_GetFlagStatus(TIM1,TIM_IT_Update) == SET)
	{
		TIM_ClearFlag(TIM1,TIM_IT_Update);
		cnt_response++;
		if( cnt_response == time_response)
		{
			ADC_StartOfConversion(ADC1);
			cnt_response = 0;
		}
		//TIM_Cmd(TIM3,ENABLE);
//		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_4) == SET)
//		{
//			//GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//		}
//		else
//		{
//			//GPIO_SetBits(GPIOA,GPIO_Pin_4);
//		}
	}


}
void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_IT_CC4) == SET)
	{
		TIM_ClearFlag(TIM1,TIM_IT_CC4);
		//TIM_Cmd(TIM3,DISABLE);
//		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_4) == SET)
//		{
//			GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//		}
//		else
//		{
//			GPIO_SetBits(GPIOA,GPIO_Pin_4);
//		}
	}


}
