#include "bsp_dht11.h"


static void dht11_set_level(dht11_device_t *dev,uint8_t level)
{
	hal_gpio_write(dev->gpio_port,dev->gpio_pin,level);
}

void dht11_init(dht11_driver_t *self)
{
		gpio_clock_enable(self->dev->gpio_port);
		GPIO_InitTypeDef GPIO_InitStruct={0};
		GPIO_InitStruct.Pin=self->dev->gpio_pin;
		GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed=GPIO_SPEED_LOW;
		HAL_GPIO_Init(self->dev->gpio_port,&GPIO_InitStruct);
}



//等待电平+超时 函数
static int dht11_wait_level(dht11_device_t *dev, uint8_t level, uint32_t timeout_us)
{
    while (timeout_us--)
    {
        if (hal_gpio_read(dev->gpio_port, dev->gpio_pin) == level)
            return 0;
        hal_delay_us(1);
    }
    return -1; // 超时
}


static int dht11_start(dht11_device_t *dev)
{
	
	// 将GPIO设为输出模式 
	dev->gpio_port->MODER &= ~(0x3 << (5*2));   // 先清零
	dev->gpio_port->MODER |=  (0x1 << (5*2));   // 设置为输出
	
	
	
	dht11_set_level(dev,0);  //先将引脚电平拉低
	
	hal_delay_ms(20);        //最低延迟18ms，这里我们延迟20ms

	dht11_set_level(dev,1);  //将引脚电平拉高
	
	// 将GPIO切为输入模式 
	dev->gpio_port->MODER &= ~(0x3 << (5*2));   // MODER5 = 00 → 输入

	
	if(dht11_wait_level(dev,0,100)<0) return -1;  //等拉低
	
	if(dht11_wait_level(dev,1,100)<0) return -1;  //等拉高
	
	if(dht11_wait_level(dev,0,100)<0) return -1;  //等拉低
	
}


static int dht11_readbit(dht11_device_t *dev)
{
	
	
	if(dht11_wait_level(dev,1,100)<0) return -1;
	hal_delay_us(35);
	
	int bit=hal_gpio_read(dev->gpio_port,dev->gpio_pin);

	if(dht11_wait_level(dev,0,100)<0) return -1;
	return bit;

}

void dht11_read(dht11_driver_t *self,uint8_t* buf)
{
	dht11_start(self->dev);
	for(uint8_t x=0;x<5;x++)
	{
		buf[x]=0;
		for(uint8_t y=0;y<8;y++)
		{
			if(dht11_readbit(self->dev)==1)
			{
					buf[x]=(buf[x]<<1) | 1;
			}
			else
			{
				buf[x]=(buf[x]<<1) | 0;
			}
		}
	}
	
	
}
	

void dht11_driver_register(dht11_driver_t *self, dht11_device_t *dev)
{
	self->dev=dev;
	self->dht11_init=dht11_init;
	self->dht11_read=dht11_read;
}





















	




