#include "at.h"
#include <string.h>
#include "systick.h"
#include "esp8684_driver.h"

static char __atResponseSnapshot[1024];
void AT_ClearResponseSnapshot(void)
{
    memset(__atResponseSnapshot, 0, 1024);
}

typedef struct AT_FSM AT_FSM_t;
struct AT_FSM
{
    uint64_t sentCmdTime;
    AT_Cmd_t* currentCmd;
    COMM_STATE_t (*stateHandler)(void);
};

static COMM_STATE_t __onSend(void);
static COMM_STATE_t __onWait(void);
static COMM_STATE_t __onReceive(void);

static AT_FSM_t __atFSM = {0ULL, NULL, __onSend};

COMM_STATE_t AT_CmdHandler(AT_Cmd_t* cmd)
{
    __atFSM.currentCmd = cmd;
    return __atFSM.stateHandler();
}

static COMM_STATE_t __onSend(void)
{
    ESP8684_SendCommand(__atFSM.currentCmd->cmd);
    __atFSM.sentCmdTime  = SYSTICK_GetSysRunTime();
    __atFSM.stateHandler = __onWait;
    return COMM_STATE_PROCESSING;
}

static COMM_STATE_t __onWait(void)
{
    static uint8_t retry = 0;
    if(ESP8684_IsPacketReceived())
    {
        retry = 0;
        ESP8684_SnapshotResponse(__atResponseSnapshot);
        __atFSM.stateHandler = __onReceive;
        return COMM_STATE_PROCESSING;
    }
    else if((SYSTICK_GetSysRunTime() - __atFSM.sentCmdTime) >= __atFSM.currentCmd->timeoutMs)
    {
        __atFSM.stateHandler = __onSend;
        if(retry < __atFSM.currentCmd->maxRetry)
        {
            retry++;
            return COMM_STATE_PROCESSING;
        }
        else
        {
            retry              = 0;
            __atFSM.currentCmd = NULL;
            return COMM_STATE_FAILED_TIMER;
        }
    }
    return COMM_STATE_PROCESSING;
}

static COMM_STATE_t __onReceive(void)
{
    static uint8_t retry = 0;
    if(strstr(__atResponseSnapshot, __atFSM.currentCmd->desiredResponse) != NULL)
    {
        __atFSM.stateHandler = __onSend;
        retry                = 0;
        return COMM_STATE_OK;
    }
    else
    {
        __atFSM.stateHandler = __onSend;
        if(retry < __atFSM.currentCmd->maxRetry)
        {
            retry++;
            return COMM_STATE_PROCESSING;
        }
        else
        {
            retry              = 0;
            __atFSM.currentCmd = NULL;
            return COMM_STATE_FAILED_RESPONSE;
        }
    }
    return COMM_STATE_PROCESSING;
}

typedef enum
{
    AT_RST,
    AT_RST_DELAY,
    AT_E0,
    AT_CWMODE_1,
} AT_INIT_CMD_INDEX_t;

static const AT_Cmd_t __AT_INIT_CMD[] = {
    [AT_RST] =
        {
            .cmd             = "AT+RST\r\n",
            .desiredResponse = "OK",
            .timeoutMs       = 500,
            .maxRetry        = 3,
        },
    [AT_RST_DELAY] =
        {
            .cmd             = NULL,
            .desiredResponse = "deadbeaf",
            .timeoutMs       = 2000,
            .maxRetry        = 0,
        },
    [AT_E0] =
        {
            .cmd             = "ATE0\r\n",
            .desiredResponse = "OK",
            .timeoutMs       = 500,
            .maxRetry        = 3,
        },
    [AT_CWMODE_1] =
        {
            .cmd             = "AT+CWMODE=1\r\n",
            .desiredResponse = "OK",
            .timeoutMs       = 500,
            .maxRetry        = 0,
        },
};

COMM_STATE_t AT_Init(void)
{
    COMM_STATE_t commState                = COMM_STATE_PROCESSING;
    static AT_INIT_CMD_INDEX_t atInitStep = AT_RST;

    switch(atInitStep)
    {
        case AT_RST:
            commState = AT_CmdHandler(__AT_INIT_CMD + AT_RST);
            if(commState == COMM_STATE_OK)
            {
                ClearRecvWifiStr();
                atInitStep = AT_RST_DELAY;
            }
            else if(commState == COMM_STATE_FAILED_TIMER || commState == COMM_STATE_FAILED_RESPONSE)
            {
                ClearRecvWifiStr();
                return commState;
            }
            break;
        case AT_RST_DELAY:
            commState = AT_CmdHandler(__AT_INIT_CMD + AT_RST_DELAY);
            if(commState == COMM_STATE_OK)
            {
                ClearRecvWifiStr();
                atInitStep = AT_E0;
            }
            else if(commState == COMM_STATE_FAILED_TIMER || commState == COMM_STATE_FAILED_RESPONSE)
            {
                ClearRecvWifiStr();
                atInitStep = AT_E0;
            }
            break;
        case AT_E0:
            commState = AT_CmdHandler(__AT_INIT_CMD + AT_E0);
            if(commState == COMM_STATE_OK)
            {
                ClearRecvWifiStr();
                atInitStep = AT_CWMODE_1;
            }
            else if(commState == COMM_STATE_FAILED_TIMER || commState == COMM_STATE_FAILED_RESPONSE)
            {
                ClearRecvWifiStr();
                atInitStep = AT_RST;
                return commState;
            }
            break;
        case AT_CWMODE_1:
            commState = AT_CmdHandler(__AT_INIT_CMD + AT_CWMODE_1);
            if(commState == COMM_STATE_OK)
            {
                ClearRecvWifiStr();
                atInitStep = AT_RST;
                return COMM_STATE_OK;
            }
            else if(commState == COMM_STATE_FAILED_TIMER || commState == COMM_STATE_FAILED_RESPONSE)
            {
                ClearRecvWifiStr();
                atInitStep = AT_RST;
                return commState;
            }
            break;
        default:
            break;
    }
    return COMM_STATE_PROCESSING;
}
