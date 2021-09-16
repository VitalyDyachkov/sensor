#include "main.h"
#include "stdbool.h"

//uint32_t a = 156;
uint32_t *ptr = (uint32_t*) 0x1FFFF7AC;
uint32_t id[3];
uint32_t time_response = 3;
uint32_t cnt_response = 0;
uint32_t crc_id;
extern bool flag_get_temperature;
extern bool flag_get_hum;
extern bool flag_get_can_msg;
extern uint16_t data_adc_1;
extern uint16_t data_adc_2;
void GPIO_ToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
int main(void)
{
	InitCAN();
	InitADC();
	InitTIM_1();
	//InitTIM_3();
	GPIO_InitTypeDef InitPort;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
	
	InitPort.GPIO_Mode = GPIO_Mode_OUT;
	InitPort.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	InitPort.GPIO_PuPd = GPIO_PuPd_NOPULL;
	InitPort.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&InitPort);

	for (uint32_t i = 0 ;i < 3;i++)
	{
			id[i] = ptr[i];
	}
	
	crc_id = CRC_CalcBlockCRC(id,3);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
  
	while (1)
  {
		if(flag_get_temperature)
		{
			uint16_t data;
			flag_get_temperature = false;
			data = data_adc_1;
			GPIO_ToggleBit(GPIOA,GPIO_Pin_5);
			SendDataCAN(TEMPERATURE,data);
			for (uint32_t i = 0 ;i < 100000;i++)
			{}
		}
		if(flag_get_hum)
		{
			uint16_t data;
			flag_get_hum = false;
			data = data_adc_2;
			GPIO_ToggleBit(GPIOA,GPIO_Pin_5);
			SendDataCAN(HUMIDITY,data);
			for (uint32_t i = 0 ;i < 100000;i++)
			{}
		}
		if(CAN_GetFlagStatus(CAN,CAN_FLAG_BOF) == SET)
		{
			do{
			GPIO_ToggleBit(GPIOA,GPIO_Pin_5);
			for (uint32_t i = 0 ;i < 100000;i++)
			{}
			}
			while(1);
		}
//		if(flag_get_can_msg)
//		{
//			flag_get_can_msg = false;
//			GPIO_ToggleBit(GPIOA,GPIO_Pin_5);
//			for (uint32_t i = 0 ;i < 100000;i++)
//			{}
//		}
  }
}
void GPIO_ToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->ODR ^= GPIO_Pin;
}
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
		
  }
}

