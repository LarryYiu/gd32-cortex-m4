/**
 * 1. configure GPIO
 * 2. configure EXTI
 * 3. mount EXTI line to GPIO
 * 4. configure EXTI trigger mode
 * 5. enable EXTI and set priority (NVIC)
 * 6. write EXTI IRQHandler to handle the interrupt, clear the EXTI flag bit by software in the EXTI IRQHandler
 */
#ifndef __EXTI_H__
#define __EXTI_H__

#include <stdint.h>
#include <stdbool.h>
#include "gd32f30x.h"

void EXTI_Config(void);

void EXTI_DisableAll(void);

void EXTI_Enable(exti_line_enum extiLine, uint8_t prePriority, uint8_t subPriority);
void EXTI_Disable(exti_line_enum extiLine);

#endif // __EXTI_H__
