#ifndef __INTERNAL_FLASH_H__
#define __INTERNAL_FLASH_H__

#include <stdint.h>
#include <stdbool.h>

#define FLASH_PAGE_SIZE (2048U)              // 2K
#define FLASH_DEADLINE_ADDRESS (0x08080000U) // 512K
#define FLASH_SIZE (0x80000U)                // 0x08080000 - 0x08000000

#define BOOTLOADER_AREA_SIZE (28 * 1024)
#define UPDATE_INFO_AREA_SIZE (2 * 1024)
#define PARAMETER_AREA_SIZE (2 * 1024)
#define APP_AREA_SIZE (240 * 1024)
#define DOWNLOAD_AREA_SIZE (240 * 1024)
#define APP_AREA_ADDR_IN_FLASH (0x8007000)         // must be dividaible by 2048
#define DOWNLOAD_AREA_ADDR_IN_FLASH (0x8043000)    // must be dividaible by 2048
#define UPDATE_INFO_AREA_ADDR_IN_FLASH (0x807F000) // must be dividaible by 2048

#define TEST_PARAMETER_ADDR_IN_FLASH (0x807E800U) // must be dividaible by 2048

bool FLASH_Read(uint32_t addr, uint8_t* buffer, uint32_t lenReading);

bool FLASH_Write(uint32_t addr, uint8_t* buffer, uint32_t lenWriting);

bool FLASH_Erase(uint32_t addr, uint32_t lenErasing);

#endif // __INTERNAL_FLASH_H__
