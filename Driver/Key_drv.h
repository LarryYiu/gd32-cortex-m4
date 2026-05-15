#ifndef __KEY_DRV_H__
#define __KEY_DRV_H__

#include <stdint.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "GPIO_iden.h"
#include "Config.h"

void KEY_Config(void);

void KEY_Enable(void);
void KEY_Disable(void);

void KEY_AddShortPressListener(uint8_t keyIndex, void (*listener)(void));
void KEY_AddLongPressListener(uint8_t keyIndex, void (*listener)(void));
void KEY_RemoveShortPressListener(uint8_t keyIndex);
void KEY_RemoveLongPressListener(uint8_t keyIndex);

void KEY_Scan(void);

#endif // __KEY_DRV_H__
