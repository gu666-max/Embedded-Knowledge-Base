#include "hal_delay.h"


/*
	初始化DWT
*/
static void dwt_init(void)
{
	  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;	
}


void hal_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}


void hal_delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);
    while ((DWT->CYCCNT - start) < ticks);   
}


