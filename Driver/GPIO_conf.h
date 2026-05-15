#ifndef __GPIO_CONF__
#define __GPIO_CONF__

#include <stdint.h>
#include "gd32f30x.h"

uint32_t GPIO_GetPeriphAddr(uint8_t iden);
uint32_t GPIO_GetPinAddr(uint8_t iden);
rcu_periph_enum GPIO_GetRcuPeriph(uint8_t iden);

#endif // __GPIO_CONF__
