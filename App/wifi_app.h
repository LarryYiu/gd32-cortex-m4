#ifndef __WIFI_APP_H__
#define __WIFI_APP_H__

#include "wifi_driver.h"
#include <stdint.h>

void WIFI_AppInit(void);

typedef enum WIFI_COMM_STATE WIFI_COMM_STATE_t;
enum WIFI_COMM_STATE
{
    WIFI_COMM_IDLE,
    WIFI_COMM_OK,
    WIFI_COMM_PROCESSING,
    WIFI_COMM_FAILED_TIMER,
    WIFI_COMM_FAILED_RESPONSE
};

WIFI_COMM_STATE_t WIFI_CmdHandler(const char* cmd, uint32_t timeoutMs, uint8_t retryCount);

void WIFI_Idle(void);

#endif // __WIFI_APP_H__
