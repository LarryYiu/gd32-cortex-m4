/**
 * Code is provided by the course instructor
 */

#include "gd32f30x.h"
#include "internal_flash.h"

typedef void (*pFunction)(void);

#define RAM_START_ADDRESS 0x20000000
#define RAM_SIZE 0x10000
void BootToApp(void)
{
    uint32_t resetHandlerAddr = *(uint32_t*)(APP_AREA_ADDR_IN_FLASH + 4);
    uint32_t stackTopAddr     = *(uint32_t*)APP_AREA_ADDR_IN_FLASH;

    if(stackTopAddr > RAM_START_ADDRESS &&
       stackTopAddr < (RAM_START_ADDRESS + RAM_SIZE)) // validate the stack top address is within the valid RAM range
    {
        __disable_irq(); // disable all interrupts

        __set_MSP(stackTopAddr);
        nvic_vector_table_set(
            NVIC_VECTTAB_FLASH,
            APP_AREA_ADDR_IN_FLASH - NVIC_VECTTAB_FLASH); // reconfigure the interrupt vector table for the application
        ((pFunction)resetHandlerAddr)();
    }
}
