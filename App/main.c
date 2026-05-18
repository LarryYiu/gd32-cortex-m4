#include <stdio.h>

#include "led_driver.h"
#include "exti.h"
#include "uart.h"

int main(void)
{
    LED_Config();
    UART_Config(115200U);

    while(1)
    {
        UART0_Task();
    }
}
