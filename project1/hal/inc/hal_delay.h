#ifndef __HAL_DELAY_H
#define __HAL_DELAY_H
#include "hal.h"
#include "stm32f4xx_hal.h"

void hal_delay_ms(uint32_t ms);
void hal_delay_us(uint32_t us);
#endif