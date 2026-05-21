#ifndef __TIMER_H__
#define __TIMER_H__
#include <stdint.h>
#include <stdbool.h>

void TIMER_Init(void);

void PWM_Test(void);

uint32_t TIMER5_GetCount(void);

#endif // __TIMER_H__
