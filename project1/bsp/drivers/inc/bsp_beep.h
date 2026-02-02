#ifndef __BSP_BEEP_H

#define __BSP_BEEP_H
#include "bsp.h"

typedef struct beep_device
{
	uint8_t id;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t active_level;   //这个状态是引脚输出什么电平才会点亮,也就是有效电平
	uint8_t init_level;
}beep_device_t;


typedef struct beep_driver
{
	beep_device_t *dev;
	void (*beep_init)(struct beep_driver *self);
	void (*beep_on)(struct beep_driver *self);
	void (*beep_off)(struct beep_driver *self);
}beep_driver_t;

//函数声明
void beep_init(struct beep_driver *self);
void beep_on(struct beep_driver *self);
void beep_off(struct beep_driver *self);
void beep_driver_register(beep_driver_t *self, beep_device_t *dev);

#endif

