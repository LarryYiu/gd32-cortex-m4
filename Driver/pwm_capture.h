#ifndef __PWM_CAPTURE_H__
#define __PWM_CAPTURE_H__
#include <stdint.h>

/**
 * @brief  Initialize the PWM capture functionality on TIMER7 channel 0 and GPIOC pin 6.
 *         This function configures the necessary GPIO and TIMER settings to enable PWM signal capture.
 */
void PWM_CAP_Init(void);
#endif // __PWM_CAPTURE_H__
