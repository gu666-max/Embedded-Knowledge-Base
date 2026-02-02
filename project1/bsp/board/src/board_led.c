#include "board_led.h"


            /*          LED1             */
static led_device_t led1_dev={1,GPIOF,GPIO_PIN_9,0,1,1};
static led_driver_t led1_drv;

					/*          LED2             */
static led_device_t led2_dev={1,GPIOF,GPIO_PIN_10,0,1,1};
static led_driver_t led2_drv;

static int led_dev_init(device_t *dev)
{
	
	led_driver_t *led_drv=(led_driver_t*)dev->user_data;
	
	led_drv->led_init(led_drv);
	
}

static int led_dev_write(device_t *dev,int cmd)
{
	led_driver_t *led_drv=(led_driver_t*)dev->user_data;
	
	if(cmd==0)
		led_drv->led_on(led_drv);
	else if(cmd==1)
		led_drv->led_off(led_drv);	
	else if(cmd==3)
		led_drv->led_toggle(led_drv);
	
}


						/*          LED1             */
static device_ops_t led1_ops={
	.init=led_dev_init,
	.write=led_dev_write,
	.open=NULL,
	.close=NULL,
	.read=NULL
};

static device_t led1_device={
	.name="led1",
	.ops=&led1_ops,
	.user_data=&led1_drv,
	.opened=0,
};



						/*          LED2             */
static device_ops_t led2_ops={
	.init=led_dev_init,
	.write=led_dev_write,
	.open=NULL,
	.close=NULL,
	.read=NULL
};

static device_t led2_device={
	.name="led2",
	.ops=&led2_ops,
	.user_data=&led2_drv,
	.opened=0,
};


void board_led_init(void)
{
						/*          LED1             */	
    led_driver_register(&led1_drv, &led1_dev);
    device_register(&led1_device);
	
						/*          LED2             */
	  led_driver_register(&led2_drv, &led2_dev);
    device_register(&led2_device);
	
}


	