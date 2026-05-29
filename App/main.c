#include <stdio.h>

#include "systick.h"
#include "uart.h"
#include "esp8684_driver.h"
#include "wifi_app.h"
#include "rtc.h"

int main(void)
{
    SYSTICK_Config();
    UART_Config(115200U);
    ESP8684_Init();
    RTC_Init();
    // static uint64_t lastPrintTime = 0;
    // static RTC_Time_t currentTime;
    while(1)
    {
        WIFI_Run();
        // if(SYSTICK_GetSysRunTime() - lastPrintTime >= 1000)
        // {
        //     lastPrintTime = SYSTICK_GetSysRunTime();

        //     RTC_GetTime(&currentTime);
        //     printf("Current Time: %04d-%02d-%02d %02d:%02d:%02d\n", currentTime.year, currentTime.month,
        //            currentTime.day, currentTime.hour, currentTime.minute, currentTime.second);
        // }
    }
}
