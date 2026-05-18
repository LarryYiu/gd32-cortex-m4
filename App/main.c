#include "led_driver.h"
#include "dwt_delay.h"
#include "exti.h"

int main(void)
{
    LED_Config();
    DWT_Config();

    while(1)
    {
    }
}
