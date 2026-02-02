#include "device_manager.h"

#define DEVICE_MAX 8
static device_t *device_table[DEVICE_MAX];  // 系统所有设备



int device_register(device_t *dev)
{
	
	if(!dev || !dev->name || !dev->ops)
		return -1;  //注册失败
	
	
	for(uint8_t i=0;i<DEVICE_MAX;i++)  //检查是否已经注册过
	{
			if(device_table[i] && strcmp(device_table[i]->name, dev->name) == 0)
					return -3; // 已注册
	}
	
	
	for(uint8_t i=0;i<DEVICE_MAX;i++)  //注册
	{
		if(device_table[i]==NULL)
		{
			device_table[i]=dev;
			return 0;  //注册成功
		}
	}
	
	return -2;  //表满
	
}

device_t *device_find(const char *name)
{
	
		for(uint8_t i=0;i<DEVICE_MAX;i++)
		{
			if(device_table[i] && strcmp(device_table[i]->name,name)==0)
			{
				return device_table[i];
			}
		}
		
		return NULL;
}


int device_open(const char *name)
{
	device_t *dev=device_find(name);
	
	if(!dev || !dev->ops || !dev->ops->init)
		return -1;
	
	if(!dev->opened)
	{
		dev->ops->init(dev);
		dev->opened=1;
	}
	
	return 0;
}

int device_close(const char *name)
{
	device_t *dev=device_find(name);
	
	if(!dev || !dev->opened)
		return -1;
	
	if(dev->ops&&dev->ops->close)
	{
		dev->ops->close(dev);
	}
	
	dev->opened=0;
	
	return 0;
}


int device_write(const char *name, int cmd)
{
	device_t *dev=device_find(name);
	
	if(!dev || !dev->ops || !dev->ops->write || !dev->opened)
		return -1;
	
	return dev->ops->write(dev,cmd);
}

int device_read(const char *name, void *buf)
{
	device_t *dev=device_find(name);
	
	if(!dev || !dev->ops || !dev->ops->read)
		return -1;
	
	return dev->ops->read(dev,buf);
}





/* ================= 外部函数 ================= */

