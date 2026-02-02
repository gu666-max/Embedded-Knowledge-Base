#include "bsp_key.h"




static int key_read_gpio(key_device_t *dev)
{
	int level=0;
	level=hal_gpio_read(dev->key_port,dev->key_pin);
	return level;
}


void key_init(key_driver_t *self)
{  
	gpio_clock_enable(self->dev->key_port);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.Pin=self->dev->key_pin;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(self->dev->key_port,&GPIO_InitStruct);
	self->dev->active_level=KEY_IDLE;
	self->dev->time=0;
	self->dev->key_event=NULL_PRESS;
}


key_event_t key_scan(key_driver_t *self)
{
	int level=hal_gpio_read(self->dev->key_port,self->dev->key_pin);
	
	switch(self->dev->key_state)
	{
		// 1.空闲
		case KEY_IDLE:
		{
			if(level==self->dev->active_level)
			{
				self->dev->key_state=KEY_DOWN_SHAKE;
				self->dev->time=0;
			}
			break;
		}
			
		//2.按下消抖
		case KEY_DOWN_SHAKE:
		{
			if(level!=self->dev->active_level)
			{
				self->dev->key_state=KEY_IDLE;
				self->dev->time=0;
			}
			else
			{
				if((++self->dev->time)>=KEY_SHAKE_TIME)
				{
					self->dev->key_state=KEY_DOWN;
					self->dev->time=0;
				}
			}
			break;
		}
		
		//3.稳定按下
		case KEY_DOWN:
		{
			if(level!=self->dev->active_level)
			{
				//短按
				self->dev->key_state=KEY_UP_SHAKE;
				self->dev->time=0;
				self->dev->prev_state=KEY_DOWN;
			}
			else
			{
				//长按
				if((++self->dev->time)>=KEY_LONG_TIME)
				{
					self->dev->key_state=KEY_HOLD;
					self->dev->time=0;
				}
			}
			break;
		}
		
		//4.长按
		case KEY_HOLD:
		{
			if(level!=self->dev->active_level)
			{
				self->dev->key_state=KEY_UP_SHAKE;
				self->dev->prev_state=KEY_HOLD;
				self->dev->time=0;
			}
			break;
		}
		
		// 5.抬起消抖
		case KEY_UP_SHAKE:
		{
			if(level==self->dev->active_level)
			{
				self->dev->key_state=self->dev->prev_state;
				self->dev->time=0;
			}
			else
			{
				if((++self->dev->time)>=KEY_SHAKE_TIME)
				{
					switch(self->dev->prev_state)
					{
						case KEY_DOWN :
							self->dev->key_event=SHORT_PRESS;
							break;
						case KEY_HOLD:
							self->dev->key_event=LONG_PRESS;
							break;
						
						
							
					}
					self->dev->time=0;
					self->dev->key_state=KEY_IDLE;
						
					self->dev->key_event_huan=self->dev->key_event;
					self->dev->key_event=NULL_PRESS;
				}
			}
			break;
		}
			
	}
	
	return self->dev->key_event_huan;
	
}







/*
	设备绑定
*/
void key_driver_register(key_driver_t *self, key_device_t *dev)
{
	self->dev=dev;
	self->key_init=key_init;
	self->key_scanf=key_scan;

}










