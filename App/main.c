#include "led_driver.h"
#include "dwt_delay.h"

int main(void)
{
    LED_Config();
    DWT_Config();
    Led_t led0 = LED_GetLedStruct(0); // Get LED A8
    Led_t led1 = LED_GetLedStruct(1); // Get LED E6
    Led_t led2 = LED_GetLedStruct(2); // Get LED F6

    while(1)
    {
        // LED A8 on
        LED_SetState(&led0, SET);
        DWT_Delay_ms(500);
        LED_SetState(&led0, RESET);

        // LED E6 on
        LED_SetState(&led1, SET);
        DWT_Delay_ms(500);
        LED_SetState(&led1, RESET);

        // LED F6 on
        LED_SetState(&led2, SET);
        DWT_Delay_ms(500);
        LED_SetState(&led2, RESET);
    }
}
