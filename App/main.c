#include <stdio.h>

#include "systick.h"
#include "uart.h"
#include "esp8684_driver.h"
#include "wifi_app.h"
#include "rtc.h"
#include "internal_flash.h"

void printStr(uint32_t addr, char* str, uint8_t len)
{
    FLASH_Read(addr, (uint8_t*)str, len);
    for(uint8_t i = 0; i < len; i++)
    {
        printf("%02x ", str[i]);
    }
    printf("\r\n");
}

int main(void)
{
    SYSTICK_Config();
    UART_Config(115200U);
    // ESP8684_Init();

    // char str[128];
    // char s1[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a};

    // printf("================CLear flash======================\r\n");
    // FLASH_Erase(PARAMETER_ADDR_IN_FLASH, FLASH_PAGE_SIZE);
    // FLASH_Erase(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);
    // FLASH_Erase(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE * 2, FLASH_PAGE_SIZE);
    // printf("================Write Initial Data===============\r\n");
    // FLASH_Write(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE - 10, (uint8_t*)s1, (sizeof(s1) / sizeof((s1)[0])));
    // printStr(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE - 10, str, 10);
    // FLASH_Write(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE, (uint8_t*)s1, (sizeof(s1) / sizeof((s1)[0])));
    // printStr(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE, str, 10);
    // FLASH_Write(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE * 2, (uint8_t*)s1, (sizeof(s1) / sizeof((s1)[0])));
    // printStr(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE * 2, str, 10);
    // printf("================Case 1======================\r\n");
    // FLASH_Erase(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE - 2, 4 + 2048);
    // printf("================page 1======================\r\n");
    // printStr(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE - 10, str, 10);
    // printf("================page 2======================\r\n");
    // printStr(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE, str, 10);
    // printf("================page 3======================\r\n");
    // printStr(PARAMETER_ADDR_IN_FLASH + FLASH_PAGE_SIZE * 2, str, 10);
    // printf("================End======================\r\n");
    while(1)
    {
        // WIFI_Run();
    }
}
