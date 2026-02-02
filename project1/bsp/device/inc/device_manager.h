#ifndef DEVICE_MANAGER_H

#define DEVICE_MANAGER_H

#include "stm32f4xx_hal.h"  //引入芯片的头文件
#include "string.h"
typedef struct device device_t;
typedef struct device_ops device_ops_t;

struct device_ops
{
	int (*init)(device_t *dev);
	int (*open)(device_t*dev);
	int (*close)(device_t *dev);
	int (*write)(device_t*dev,int cmd);
	int (*read)(device_t *dev,void* buf);
};


struct device
{
	const char *name;
	const device_ops_t  *ops;
	void *user_data;
	uint8_t opened;
	void (*callback)(struct device *dev, void *event); // 应用回调
};


/* ================= 对外接口 ================= */
int device_register(device_t *dev);


device_t *device_find(const char *name);


int device_open(const char *name);
int device_close(const char *name);


int device_write(const char *name, int cmd);
int device_read(const char *name, void *buf);

#endif



