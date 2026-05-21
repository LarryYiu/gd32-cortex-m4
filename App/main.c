#include <stdio.h>

#include "systick.h"
#include "led_driver.h"
#include "uart.h"
#include "key_driver.h"
#include "timer.h"

void onShortPress(void);
void onLongPress(void);
void onContinuousPress(uint8_t count);

int main(void)
{
    SYSTICK_Config();
    // LED_Config();
    UART_Config(115200U);
    KEY_Config();
    TIMER_Init();

    KEY_AddShortPressListener(0, onShortPress);
    KEY_AddLongPressListener(0, onLongPress);
    KEY_AddContinuousPressListener(0, onContinuousPress);
    while(1)
    {
        // KEY_Scan(0);
        PWM_Test();
    }
}

void onShortPress(void)
{
    LED_Toggle(0);
}

void onLongPress(void)
{
    LED_SetState(0, RESET);
}

void onContinuousPress(uint8_t count)
{
    printf("ContinuousPress: %hhu\n", count);
}
