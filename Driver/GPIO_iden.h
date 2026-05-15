/**
 * @file GPIO_conf.h
 *
 * @note the GPIO pin identifier (named 'iden') is defined as follow:
 * *  - bit[7:4] is the GPIO port index, which is used to calculate the GPIO peripheral address
 * *  - bit[3:0] is the GPIO pin index, which is used to calculate the GPIO pin bit mask
 */

#ifndef __GPIO_CONF__
#define __GPIO_CONF__

#include <stdint.h>
#include "gd32f30x.h"

#define GPIO_IDEN_PORT_A 0
#define GPIO_IDEN_PORT_B 1
#define GPIO_IDEN_PORT_C 2
#define GPIO_IDEN_PORT_D 3
#define GPIO_IDEN_PORT_E 4
#define GPIO_IDEN_PORT_F 5
#define GPIO_IDEN_PORT_G 6
#define GPIO_IDEN_PORT_a 0
#define GPIO_IDEN_PORT_b 1
#define GPIO_IDEN_PORT_c 2
#define GPIO_IDEN_PORT_d 3
#define GPIO_IDEN_PORT_e 4
#define GPIO_IDEN_PORT_f 5
#define GPIO_IDEN_PORT_g 6

#define GPIO_Ident(port, pin) (((GPIO_IDEN_PORT_##port) << 4) | (pin))

/**
 * **********************************************************************
 * @brief get the GPIO peripheral address according to the GPIO pin identifier
 * the GPIO peripheral address is used config the GPIO with `gpio_init()`
 * @param iden the GPIO pin identifier
 * @return the GPIO peripheral address
 * **********************************************************************
 */
uint32_t GPIO_GetPeriphAddr(uint8_t iden);

/**
 * **********************************************************************
 * @brief get the GPIO pin bit mask according to the GPIO pin identifier
 * the GPIO pin bit mask is used to configure the GPIO pin with `gpio_init()`
 * @param iden the GPIO pin identifier
 * @return the GPIO pin bit mask
 * **********************************************************************
 */
uint32_t GPIO_GetPinAddr(uint8_t iden);

/**
 * **********************************************************************
 * @brief get the RCU peripheral according to the GPIO pin identifier
 * the RCU peripheral is used to enable the GPIO clock with `rcu_periph_clock_enable()`
 *
 * @param iden the GPIO pin identifier
 * @return `rcu_periph_enum` the RCU peripheral
 * **********************************************************************
 */
rcu_periph_enum GPIO_GetRcuPeriph(uint8_t iden);
#endif // __GPIO_CONF__
