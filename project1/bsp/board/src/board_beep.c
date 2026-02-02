#include "board_beep.h"

static beep_device_t beep1_dev={1,GPIOF,GPIO_PIN_8,1,0};
static beep_driver_t beep1_drv;

static int beep_dev_init(device_t *dev)
{
	beep_driver_t *beep_drv=(beep_driver_t*)dev->user_data;
	beep_drv->beep_init(beep_drv);
}

static int beep_dev_write(device_t *dev,int cmd)
{
	beep_driver_t *beep_drv=(beep_driver_t*)dev->user_data;
	if(cmd)
		beep_drv->beep_on(beep_drv);
	else
		beep_drv->beep_off(beep_drv);
}

static device_ops_t beep1_ops={
	.init=beep_dev_init,
	.write=beep_dev_write,
	.open=NULL,
	.close=NULL,
	.read=NULL
};

static device_t beep1_device={
	.name="beep1",
	.ops=&beep1_ops,
	.user_data=&beep1_drv,
	.opened=0
};

void board_beep_init(void)
{
	beep_driver_register(&beep1_drv,&beep1_dev);
	device_register(&beep1_device);
}