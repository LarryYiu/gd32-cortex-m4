#ifndef __WIFI_DRIVER_H__
#define __WIFI_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>

void WIFI_Init(void);

void WIFI_EnableModule(void);

void WIFI_DisableModule(void);

bool WIFI_IsPacketReceived(void);

void WIFI_SendCommand(const char* cmd);

void WIFI_SnapshotResponse(char* buffer);

#endif // __WIFI_DRIVER_H__
