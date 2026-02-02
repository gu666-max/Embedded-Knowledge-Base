#include "bsp_led.h"

/* ================= 内部函数 ================= */
static void led_set_level(led_device_t* dev,uint8_t level)
{
	hal_gpio_write(dev->led_port,dev->led_pin,level);
}



static void led_init(led_driver_t *self)
{
	gpio_clock_enable(self->dev->led_port);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.Pin=self->dev->led_pin;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(self->dev->led_port,&GPIO_InitStruct);
	led_set_level(self->dev,self->dev->init_level);
}


static void led_on(led_driver_t *self)
{
	if(self->dev->active_level==1)
	{
		led_set_level(self->dev,1);
		self->dev->current_state=1;
	}
	else
	{
		led_set_level(self->dev,0);
		self->dev->current_state=0;
	}
}


static void led_off(led_driver_t *self)
{
	if(self->dev->active_level==1)
	{
		led_set_level(self->dev,0);
		self->dev->current_state=0;
	}
	else
	{
		led_set_level(self->dev,1);
		self->dev->current_state=1;
	}
}





static void led_toggle(led_driver_t *self)
{
	if(self->dev->current_state)
	{
		led_set_level(self->dev,0);
		
		self->dev->current_state=0;
	}
	else
	{
		led_set_level(self->dev,1);
		self->dev->current_state=1;
	}
}


/* ================= 外部函数 ================= */

/*
	设备绑定
*/
void led_driver_register(led_driver_t *self, led_device_t *dev)
{
	
	self->dev=dev;
	self->led_init=led_init;
	self->led_off=led_off;
	self->led_on=led_on;
	self->led_toggle=led_toggle;
}