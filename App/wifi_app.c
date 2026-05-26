#include "wifi_app.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "systick.h"

void WIFI_AppInit(void)
{
    WIFI_Init();
}

static char __snapshot[1024];

typedef struct WifiCommFSM WifiCommFSM_t;
struct WifiCommFSM
{
    WIFI_COMM_STATE_t currentWifiState;
    bool isModuleReady;
    uint32_t timeoutMs;
    uint32_t startSendingTime;
    uint8_t retryCount;
    const char* cmd;
    void (*stateHandler)(void);
};

static void __onIdle(void);
static void __onSend(void);
static void __onWait(void);
static void __onReceive(void);

static WifiCommFSM_t __wifiCommFSM = {WIFI_COMM_IDLE, false, 0, 0, 0, NULL, __onSend};

WIFI_COMM_STATE_t WIFI_CmdHandler(const char* cmd, uint32_t timeoutMs, uint8_t retryCount)
{
    __wifiCommFSM.cmd        = cmd;
    __wifiCommFSM.timeoutMs  = timeoutMs;
    __wifiCommFSM.retryCount = retryCount;
    __wifiCommFSM.stateHandler();
    return __wifiCommFSM.currentWifiState;
}
static const char __CMD_AT[] = "AT\r\n";
static void __onSend(void)
{
    if(!__wifiCommFSM.isModuleReady)
    {
        WIFI_SendCommand(__CMD_AT);
    }
    else
    {
        WIFI_SendCommand(__wifiCommFSM.cmd);
    }
    __wifiCommFSM.startSendingTime = SYSTICK_GetSysRunTime();
    __wifiCommFSM.currentWifiState = WIFI_COMM_PROCESSING;
    __wifiCommFSM.stateHandler     = __onWait;
}

static void __onWait(void)
{
    static uint8_t retry = 0;
    if(WIFI_IsPacketReceived())
    {
        retry = 0;
        WIFI_SnapshotResponse(__snapshot);
        __wifiCommFSM.stateHandler     = __onReceive;
        __wifiCommFSM.startSendingTime = 0;
    }
    else if((SYSTICK_GetSysRunTime() - __wifiCommFSM.startSendingTime) >= __wifiCommFSM.timeoutMs)
    {
        if(retry < __wifiCommFSM.retryCount)
        {
            retry++;
            __wifiCommFSM.stateHandler = __onSend;
        }
        else
        {
            retry                          = 0;
            __wifiCommFSM.currentWifiState = WIFI_COMM_FAILED_TIMER;
            __wifiCommFSM.stateHandler     = __onIdle;
        }
    }
}

static void __onReceive(void)
{
    static uint8_t retry = 0;
    if(strstr(__snapshot, "OK") != NULL)
    {
        if(!__wifiCommFSM.isModuleReady)
        {
            __wifiCommFSM.isModuleReady = true;
            __wifiCommFSM.stateHandler  = __onSend;
        }
        else
        {
            __wifiCommFSM.currentWifiState = WIFI_COMM_OK;
            __wifiCommFSM.stateHandler     = __onIdle;
        }
        retry = 0;
    }
    else
    {
        if(retry < __wifiCommFSM.retryCount)
        {
            retry++;
            __wifiCommFSM.stateHandler = __onSend;
        }
        else
        {
            retry                          = 0;
            __wifiCommFSM.currentWifiState = WIFI_COMM_FAILED_RESPONSE;
            __wifiCommFSM.stateHandler     = __onIdle;
        }
    }
}

static void __onIdle(void)
{
    return;
}

void WIFI_Idle(void)
{
    __wifiCommFSM.currentWifiState = WIFI_COMM_IDLE;
}
