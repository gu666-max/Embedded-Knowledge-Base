#include "board_dht11.h"

static dht11_device_t dht111_dev={1,GPIOE,GPIO_PIN_5};
static dht11_driver_t dht111_drv;

static int dht11_dev_init(device_t *drv)
{
	dht11_driver_t *dht11_drv=(dht11_driver_t*)drv->user_data;
	dht11_drv->dht11_init(dht11_drv);
}

static int dht11_dev_read(device_t *drv,uint8_t *buf)
{
	dht11_driver_t *dht11_drv=(dht11_driver_t*)drv->user_data;
	dht11_drv->dht11_read(dht11_drv,buf);
}

static device_ops_t dht11_ops={
	.init=dht11_dev_init,
	.read=dht11_dev_read,
	.write=NULL,
	.open=NULL,
	.close=NULL
};

static device_t dht11_device={
	.name="dht11",
	.ops=&dht11_ops,
	.user_data=&dht111_drv,
	.opened=0,
};

void board_dht11_init(void)
{
	dht11_driver_register(&dht111_drv,&dht111_dev);
	device_register(&dht11_device);
}



