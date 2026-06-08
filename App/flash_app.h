#ifndef __FLASH_APP_H__
#define __FLASH_APP_H__

#include <stdint.h>
#include <stdbool.h>
#include "internal_flash.h"

#define SYSPARAM_MAX_SIZE FLASH_PAGE_SIZE
#define STORE_MAGIC_CODE (0xdeadbeef)

typedef struct SysParam SysParam_t;

struct SysParam
{
    uint32_t magicCode; // it is used to determine if the block is touched
    /* add SYS CONFIG starts  */
    char sysVersion[10];
    /* add SYS CONFIG ends */
    uint8_t crcVal; // validate the flash val
};

void SYS_PARAM_Init(void);
void SYS_PARAM_GetSysVersion(char* version);
bool SYS_PARAM_SetSysVersion(char* version);

#endif // __FLASH_APP_H__
