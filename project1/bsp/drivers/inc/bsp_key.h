#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "bsp.h"


//10ms调用一次
#define KEY_SHAKE_TIME     3     // 30ms
#define KEY_LONG_TIME      100   // 1000ms

typedef struct key_driver key_driver_t;


typedef enum
{
    KEY_IDLE = 0,        // 未按
    KEY_DOWN_SHAKE,      // 按下消抖
    KEY_DOWN,            // 已按下
    KEY_HOLD,            // 长按
    KEY_UP_SHAKE,        // 松开消抖
} key_state_t;

typedef enum
{
		NULL_PRESS,
		SHORT_PRESS,
		LONG_PRESS,
}key_event_t;

typedef struct key_device
{
    uint8_t id;
		GPIO_TypeDef *key_port;
		uint16_t key_pin;
		uint8_t active_level;
		key_state_t key_state;
		key_state_t	prev_state;
		key_event_t key_event;
		key_event_t key_event_huan;
		uint32_t time;
}key_device_t;


struct key_driver
{
    key_device_t* dev;
		void (*key_init)(key_driver_t* self);
		key_event_t (*key_scanf)(key_driver_t* self);
		
	
};











void key_init(key_driver_t *self);
key_event_t key_scan(key_driver_t *self);
void key_driver_register(key_driver_t *self, key_device_t *dev);


#endif