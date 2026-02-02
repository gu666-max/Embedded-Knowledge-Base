#ifndef __BSP_DHT11_H

#define __BSP_DHT11_H
#include "bsp.h"


typedef struct dht11_device
{
	uint8_t id;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
}dht11_device_t;

typedef struct dht11_driver
{
	dht11_device_t *dev;
	void (*dht11_init)(struct dht11_driver* self);
	void(*dht11_read)(struct dht11_driver *self,uint8_t *buf);
}dht11_driver_t;


void dht11_read(dht11_driver_t *self,uint8_t *buf);
void dht11_init(dht11_driver_t *self);
void dht11_driver_register(dht11_driver_t *self, dht11_device_t *dev);
#endif

