#include "hal_gpio.h"


/*
	写GPIO引脚状态
*/
void hal_gpio_write(GPIO_TypeDef *GPIOx,uint16_t gpio_pin,uint8_t pin_state)
{
	GPIO_PinState gpio_pinstate;
	if(pin_state)
		gpio_pinstate=GPIO_PIN_SET;
	else
		gpio_pinstate=GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOx,gpio_pin,gpio_pinstate);
}

/*
	读GPIO引脚状态
*/
uint8_t hal_gpio_read(GPIO_TypeDef *GPIOx,uint16_t gpio_pin)
{
	return (uint8_t)HAL_GPIO_ReadPin(GPIOx,gpio_pin);
}


/*
	GPIO时钟使能
*/
void gpio_clock_enable(GPIO_TypeDef *GPIOx)
{
	if(GPIOx==GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(GPIOx==GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(GPIOx==GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if(GPIOx==GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if(GPIOx==GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if(GPIOx==GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
}


