#include "board_key.h"

static key_device_t key1_dev={1,GPIOE,GPIO_PIN_4,0,KEY_IDLE,NULL_PRESS,0};
static key_driver_t key1_drv;

void key_trigger_event(device_t *device, key_event_t *event) {
    if(*event != NULL_PRESS) {
        device->callback(device,event);
				*event=NULL_PRESS;
    }
}

static int key_dev_init(device_t *dev)
{
	key_driver_t *drv=(key_driver_t*)dev->user_data;
	drv->key_init(drv);
}


static int key_dev_write(device_t *dev,int cmd)
{
	key_driver_t *drv=(key_driver_t*)dev->user_data;
	if(cmd==0)
		drv->key_scanf(drv);
		key_trigger_event(dev,&drv->dev->key_event_huan);
	
}

static device_ops_t key_ops={
	.init=key_dev_init,
	.write=key_dev_write,
	.open=NULL,
	.close=NULL,
	.read=NULL
};

__WEAK void my_key_handler(struct device *dev, void *event)
{
	__NOP();
}

static device_t key1_device={
	.name="key1",
	.ops=&key_ops,
	.user_data=&key1_drv,
	.opened=0,
	.callback=my_key_handler,
};

void board_key_init(void)
{
    key_driver_register(&key1_drv, &key1_dev);
    device_register(&key1_device);
}



