#include "flash_app.h"

static SysParam_t sysParamCurrent;

static const SysParam_t sysParamDefault = {
    .magicCode  = STORE_MAGIC_CODE,
    .sysVersion = "1.0",
};

void SYS_PARAM_Init(void) {}

void SYS_PARAM_GetSysVersion(char* version);
bool SYS_PARAM_SetSysVersion(char* version);
