#ifndef __HAL_RTC__H

#define __HAL_RTC__H
#include "stm32f4xx_hal.h"
#include "hal.h"



void HAL_RTC_GET_Time(void);
void hal_rtc_init(void);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);
#endif