#include "LED_drv.h"

static const uint8_t __LED_IDEN_LOOKUP[] = {LED_LOOK_UP};

void LED_Config(void)
{
    for(uint8_t i = 0; i < len(__LED_IDEN_LOOKUP); i++)
    {
        rcu_periph_clock_enable(GPIO_GetRcuPeriph(__LED_IDEN_LOOKUP[i]));
        gpio_init(GPIO_GetPeriphAddr(__LED_IDEN_LOOKUP[i]), GPIO_MODE_OUT_PP, LED_GPIO_FREQENCY,
                  GPIO_GetPinAddr(__LED_IDEN_LOOKUP[i]));
    }
}

void LED_Enable(void)
{
    for(uint8_t i = 0; i < len(__LED_IDEN_LOOKUP); i++)
    {
        gpio_init(GPIO_GetPeriphAddr(__LED_IDEN_LOOKUP[i]), GPIO_MODE_OUT_PP, LED_GPIO_FREQENCY,
                  GPIO_GetPinAddr(__LED_IDEN_LOOKUP[i]));
    }
}

void LED_Disable(void)
{
    for(uint8_t i = 0; i < len(__LED_IDEN_LOOKUP); i++)
    {
        gpio_init(GPIO_GetPeriphAddr(__LED_IDEN_LOOKUP[i]), GPIO_MODE_AIN, LED_GPIO_FREQENCY,
                  GPIO_GetPinAddr(__LED_IDEN_LOOKUP[i]));
    }
}

Led_t LED_GetLedStruct(uint8_t index)
{
    Led_t led;
    led.gpioIden = __LED_IDEN_LOOKUP[index];
    led.state    = RESET;
    return led;
}

void LED_SetState(Led_t *led, bit_status stateIn)
{
    gpio_bit_write(GPIO_GetPeriphAddr(led->gpioIden), GPIO_GetPinAddr(led->gpioIden), stateIn);
    led->state = stateIn;
}
