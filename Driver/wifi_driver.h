#ifndef __WIFI_DRIVER_H__
#define __WIFI_DRIVER_H__

#include <stdint.h>

void WIFI_Config(void);

void WIFI_SendData(uint8_t* data, uint8_t len);

void WIFI_Task(void);

void WIFI_ReceiveData(uint8_t* buffer, uint8_t len);

#endif // __WIFI_DRIVER_H__
