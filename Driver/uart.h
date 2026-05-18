#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "config.h"
#include "gpio_decoder.h"

// TX PP_out, RX floating/pullup input, pull up for idle high
static void UART_Config(void);

void UART_Enable(void);
void UART_Disable(void);
#endif // __UART_H__