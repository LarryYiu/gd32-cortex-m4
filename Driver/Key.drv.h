#ifndef __KEY_DRV_H__
#define __KEY_DRV_H__

#include <stdint.h>
#include <stdbool.h>
#include "gd32f30x.h"

#ifndef KEY_USE_BUSY_WAIT
#define KEY_USE_BUSY_WAIT true
#endif

struct Key
{
    uint8_t gpioIden;
    bool state;
};
typedef struct Key Key_t;

void KEY_Config(void);

void KEY_On(void);
void KEY_Off(void);

#endif // __KEY_DRV_H__