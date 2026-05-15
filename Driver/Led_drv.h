#ifndef __LED_DRV_H__
#define __LED_DRV_H__

#include <stdint.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "GPIO_conf.h"

struct Led
{
    uint8_t gpioIden;
    bit_status state;
};
typedef struct Led Led_t;

/**
 * @brief  Configures LED GPIO.
 */
void LED_Config(void);

/**
 * @brief  Get LED struct by index.
 * @param  index: LED index (0-based)
 *
 * @return Led_t struct with identifier and initial state (RESET)
 *
 * @note The index is indicated by the order of the __LED_IDEN_LOOKUP array
 */
Led_t LED_GetLedStruct(uint8_t index);

/**
 * @brief  Set the state of the given LED.
 * @param  led: Pointer to the LED struct
 * @param  stateIn: Desired state (SET for on, RESET for off)
 */
void LED_SetState(Led_t *led, bit_status stateIn);

#endif // __LED_DRV_H__
