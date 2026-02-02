#ifndef __BOART_KEY_H
#define __BOART_KEY_H
#include "board.h"
#include "device_manager.h"
#include "bsp_key.h"
static int key_dev_init(device_t *dev);
static int key_dev_write(device_t *dev,int cmd);

void board_key_init(void);

#endif