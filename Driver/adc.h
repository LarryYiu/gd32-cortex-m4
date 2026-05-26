#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>

void ADC_Init(void);

uint16_t ADC_Read(void);

float ADC_GetVres(void);

void VresDrvTest(void);

#endif // __ADC_H__
