#ifndef __INTERNAL_FLASH_H__
#define __INTERNAL_FLASH_H__

#include <stdint.h>
#include <stdbool.h>

#define FLASH_PAGE_SIZE (2048)              // 2K
#define FLASH_DEADLINE_ADDRESS (0x08080000) // 512K
#define FLASH_SIZE (0x80000)                // 0x08080000 - 0x08000000
#define PARAMETER_ADDR_IN_FLASH (0x807F800) // must be dividaible by 2048

bool FLASH_Read(uint32_t addr, uint8_t* buffer, uint32_t lenReading);

bool FLASH_Write(uint32_t addr, uint8_t* buffer, uint32_t lenWriting);

// erase any addr w/in the page to erase the whole page
bool FLASH_Erase(uint32_t addr, uint32_t lenErasing);

#endif // __INTERNAL_FLASH_H__
