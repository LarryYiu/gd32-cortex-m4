#include "ir_nec_parser.h"
#include <stdbool.h>
#include <stdio.h>

static uint8_t __parseRes[4]  = {0, 0, 0, 0};
static bool __isParseResValid = false;

void IR_Parse(uint32_t val)
{
    static bool isHeaderReceived = false;
    static uint8_t currIndex     = 0;
    // printf(isHeaderReceived ? "header 1\r\n" : "header 0\r\n");
    if(!isHeaderReceived)
    {
        if(val > IR_NEC_HEAD_MIN && val < IR_NEC_HEAD_MAX)
        {
            isHeaderReceived = true;
        }
        return;
    }

    if(currIndex == 32)
    {
        if((__parseRes[2] ^ __parseRes[3]) == 0xFF)
        {
            __isParseResValid = true;
        }
        else
        {
            printf("Verification Failed\r\n");
            __isParseResValid = false;
            __parseRes[0]     = 0;
            __parseRes[1]     = 0;
            __parseRes[2]     = 0;
            __parseRes[3]     = 0;
        }
        isHeaderReceived = false;
        currIndex        = 0;
        return;
    }
    else if(val > IR_NEC_1_MIN && val < IR_NEC_1_MAX)
    {
        __parseRes[currIndex / 8] = __parseRes[currIndex / 8] | (1 << (currIndex % 8));
        currIndex++;
    }
    else if(val > IR_NEC_0_MIN && val < IR_NEC_0_MAX)
    {
        currIndex++;
    }
}

uint8_t IR_GetParsedData(void)
{
    if(__isParseResValid)
    {
        __isParseResValid = false;
        uint8_t res       = __parseRes[2];
        __parseRes[0]     = 0;
        __parseRes[1]     = 0;
        __parseRes[2]     = 0;
        __parseRes[3]     = 0;
        return res;
    }
    return 0;
}
