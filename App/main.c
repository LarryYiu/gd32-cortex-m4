#include <stdio.h>

#include "systick.h"
#include "led_driver.h"
#include "exti.h"
#include "uart.h"
#include "wifi_driver.h"

int main(void)
{
    LED_Config();
    UART_Config(115200U);
    SYSTICK_Config();
    WIFI_Config();
    uint8_t testData[] = "AT+GMR\r\n";

    WIFI_SendData(testData, 8);
    while(1)
    {
        // UART0_Task();
        WIFI_Task();
    }
}
