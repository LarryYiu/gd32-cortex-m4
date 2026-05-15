#include "DWT.h"

void DWT_Config(void)
{
    if(!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
}

void DWT_On(void)
{
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void DWT_Off(void)
{
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
}

void DWT_Delay_us(uint32_t us)
{
    uint32_t startTick  = DWT->CYCCNT;
    uint32_t delayTicks = us * (rcu_clock_freq_get(CK_AHB) / 1000000);

    while(DWT->CYCCNT - startTick < delayTicks);
}

void DWT_Delay_ms(uint16_t ms)
{
    uint32_t startTick  = DWT->CYCCNT;
    uint32_t delayTicks = ms * (rcu_clock_freq_get(CK_AHB) / 1000);

    while(DWT->CYCCNT - startTick < delayTicks);
}
