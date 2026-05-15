#include "Led_drv.h"

// LED GPIO EDIT START
// [0:3] = Pin number range[0, 15]
// [4:7] = Port number (0-A, 1-B, 2-C, 3-D, 4-E, 5-F, 6-G)
static const uint8_t __LED_IDEN_LOOKUP[] = {
    0x08, // A8
    0x46, // E6
    0x56  // F6
};
// LED GPIO EDIT END

void LED_Config(void)
{
    for(uint8_t i = 0; i < sizeof(__LED_IDEN_LOOKUP) / sizeof(__LED_IDEN_LOOKUP[0]); i++)
    {
        rcu_periph_clock_enable(GPIO_GetRcuPeriph(__LED_IDEN_LOOKUP[i]));
        gpio_init(GPIO_GetPeriphAddr(__LED_IDEN_LOOKUP[i]), GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ,
                  GPIO_GetPinAddr(__LED_IDEN_LOOKUP[i]));
    }

    // equalvalent to:
    //  rcu_periph_clock_enable(RCU_GPIOA);
    //  rcu_periph_clock_enable(RCU_GPIOE);
    //  rcu_periph_clock_enable(RCU_GPIOF);

    // gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_8);
    // gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_6);
    // gpio_init(GPIOF, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_6);
}

Led_t LED_GetLedStruct(uint8_t index)
{
    Led_t led;
    led.gpioIden = __LED_IDEN_LOOKUP[index];
    led.state    = RESET;
    return led;
}

/**
 * @brief  Set the state of the given LED.
 *
 * @param  led: Pointer to the LED struct
 * @param  stateIn: Desired state (SET for on, RESET for off)
 */
void LED_SetState(Led_t *led, bit_status stateIn)
{
    // if(stateIn) // it is possible to greed this if statement by using bit op as well but kills readability
    // {
    //     GPIO_BOP(GPIO_GetPeriphAddr(led->gpioIden)) = GPIO_GetPinAddr(led->gpioIden);
    // }
    // else
    // {
    //     GPIO_BC(GPIO_GetPeriphAddr(led->gpioIden)) = GPIO_GetPinAddr(led->gpioIden);
    // }
    gpio_bit_write(GPIO_GetPeriphAddr(led->gpioIden), GPIO_GetPinAddr(led->gpioIden), stateIn);
    led->state = stateIn;
}
