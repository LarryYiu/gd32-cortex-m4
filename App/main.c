#include <stdio.h>

#include "systick.h"
#include "led_driver.h"
#include "uart.h"
#include "key_driver.h"
#include "timer.h"
#include "pwm_capture.h"
#include "ir_nec_parser.h"

void onShortPress(void);
void onLongPress(void);
void onContinuousPress(uint8_t count);

int main(void)
{
    SYSTICK_Config();
    LED_Config();
    UART_Config(115200U);
    KEY_Config();
    TIMER_Init();
    PWM_CAP_Init();
    uint8_t irRes = 0;

    KEY_AddShortPressListener(0, onShortPress);
    KEY_AddLongPressListener(0, onLongPress);
    KEY_AddContinuousPressListener(0, onContinuousPress);
    while(1)
    {
        // KEY_Scan(0);
        // PWM_Test();
        irRes = IR_GetParsedData();
        if(irRes)
        {
            printf("Parsed IR data: 0x%02X\n", irRes);
        }
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
