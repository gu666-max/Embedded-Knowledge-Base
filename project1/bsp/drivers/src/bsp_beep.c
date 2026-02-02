#include "bsp_beep.h"


/* ================= 内部函数 ================= */

static void beep_set_level(beep_device_t* dev,uint8_t level)
{
	hal_gpio_write(dev->gpio_port,dev->gpio_pin,level);
}



/* ================= 外部函数 ================= */
void beep_init(struct beep_driver *self)
{
	if(!self) return;
	
	gpio_clock_enable(self->dev->gpio_port);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.Pin=self->dev->gpio_pin;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(self->dev->gpio_port,&GPIO_InitStruct);
	beep_set_level(self->dev,self->dev->init_level);
}

void beep_on(struct beep_driver *self)
{
	if(!self) return;
	
	if(self->dev->active_level==1)
		beep_set_level(self->dev,1);
	else
		beep_set_level(self->dev,0);
}


void beep_off(struct beep_driver *self)
{
	if(!self) return;
	
	if(self->dev->active_level==1)
		beep_set_level(self->dev,0);
	else
		beep_set_level(self->dev,1);
}

void beep_driver_register(beep_driver_t *self, beep_device_t *dev)
{
	self->dev=dev;
	self->beep_init=beep_init;
	self->beep_on=beep_on;
	self->beep_off=beep_off;	
}