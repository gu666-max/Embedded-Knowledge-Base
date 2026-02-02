#include "hal_rtc.h"

void hal_rtc_init(void)
{
	RTC_HandleTypeDef RTC_HandleStruct={0};
	
	
	
	//1. 初始化RTC 
	RTC_HandleStruct.Init.AsynchPrediv=0x7F;
	RTC_HandleStruct.Init.HourFormat=RTC_HOURFORMAT_24;
	RTC_HandleStruct.Init.OutPut=RTC_OUTPUT_DISABLE;
	RTC_HandleStruct.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
	RTC_HandleStruct.Init.OutPutType=RTC_OUTPUT_TYPE_PUSHPULL;
	RTC_HandleStruct.Init.SynchPrediv=0x7FFF;
	
	HAL_RTC_Init(&RTC_HandleStruct);
	
	

	
	// 5.判断RTC时钟是否是第一次配置，如果是第一次配置，就需要配置时间，否则不需要
	HAL_RTCEx_BKUPRead(&RTC_HandleStruct,RTC_HandleStruct.Instance->BKP0R);
	if(RTC_HandleStruct.Instance->BKP0R!=0x5050)
	{	
		// 6. 因为我们使用的是LSE,所以就在BKP0R寄存器写0x5050
		HAL_RTCEx_BKUPWrite(&RTC_HandleStruct,RTC_HandleStruct.Instance->BKP0R,0x5050);
		
		//7.配置RTC日期
		RTC_DateTypeDef RTC_DateStruct={0};
		RTC_DateStruct.Year=26;
		RTC_DateStruct.WeekDay=8;
		RTC_DateStruct.Month=2;
		RTC_DateStruct.Date=1;
		HAL_RTC_SetDate(&RTC_HandleStruct,&RTC_DateStruct,RTC_FORMAT_BCD);
		
		//8.配置RTC时间
		RTC_TimeTypeDef RTC_TimeStruct={0};
		RTC_TimeStruct.Hours=0;
		RTC_TimeStruct.Minutes=0;
		RTC_TimeStruct.Seconds=0;
		RTC_TimeStruct.TimeFormat=RTC_HOURFORMAT12_AM;
		RTC_TimeStruct.SubSeconds=1;
		HAL_RTC_SetTime(&RTC_HandleStruct,&RTC_TimeStruct,RTC_FORMAT_BCD);
	}
	
	
	
}


void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	//2 
	__HAL_RCC_PWR_CLK_ENABLE();  //2.1 打开PWR
	HAL_PWR_EnableBkUpAccess();  //2.2 取消备份区域写保护 
	__HAL_RCC_RTC_ENABLE();      //2.3 使能RTC
	
	//3. 配置LSE并开启
	RCC_OscInitTypeDef RCC_OscInitStruct={0};
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState=RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	//4.配置RTC的时钟源
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct={0};
	RCC_PeriphCLKInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;
	RCC_PeriphCLKInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;
	HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
	
}


