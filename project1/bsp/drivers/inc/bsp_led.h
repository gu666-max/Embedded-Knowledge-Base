#ifndef __BSP_LED_H

#define __BSP_LED_H
#include "bsp.h"

#include "stm32f4xx_hal.h"  //引入芯片的头文件
typedef struct led_device
{
    uint8_t id;
		GPIO_TypeDef* led_port;
		uint16_t led_pin;
		uint8_t active_level;   //这个状态是引脚输出什么电平才会点亮,也就是有效电平
		uint8_t init_level;
		uint8_t current_state;
}led_device_t;

typedef struct led_driver
{
	led_device_t* dev;
	void (*led_init)(struct led_driver *self);
	void (*led_on)(struct led_driver *self);
	void (*led_off)(struct led_driver *self);
	void (*led_toggle)(struct led_driver *self);
}led_driver_t;

void led_driver_register(led_driver_t *self, led_device_t *dev);

#endif



