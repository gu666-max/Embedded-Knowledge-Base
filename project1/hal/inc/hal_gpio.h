#ifndef __HAL_GPIO_H

#define __HAL_GPIO_H

#include "hal.h"
/*
	函数外部声明
*/
void hal_gpio_write(GPIO_TypeDef *GPIOx,uint16_t gpio_pin,uint8_t pin_state);
uint8_t hal_gpio_read(GPIO_TypeDef *GPIOx,uint16_t gpio_pin);
void gpio_clock_enable(GPIO_TypeDef *GPIOx);
#endif