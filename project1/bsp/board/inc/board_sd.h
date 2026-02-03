#ifndef __BOARD_SD_H

#define __BOARD_SD_H
#include "board.h"
#include "string.h"
extern SD_HandleTypeDef SD_HandleStruct;
#define SD_INIT_CLK_DIV     118   // ~400kHz
#define SD_TRANS_CLK_DIV   2     // ~12MHz

uint8_t sd_init(void);
uint8_t sd_read(uint8_t *pbuffer);
uint8_t sd_write(uint8_t *pbuffer);


#endif