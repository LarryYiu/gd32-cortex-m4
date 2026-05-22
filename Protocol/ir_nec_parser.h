#ifndef __IR_NEC_PARSER_H__
#define __IR_NEC_PARSER_H__
#include <stdint.h>

#ifndef IR_NEC_HEAD_MAX
#define IR_NEC_HEAD_MAX 20000
#endif

#ifndef IR_NEC_HEAD_MIN
#define IR_NEC_HEAD_MIN 10000
#endif

#ifndef IR_NEC_0_MAX
#define IR_NEC_0_MAX 1800
#endif

#ifndef IR_NEC_0_MIN
#define IR_NEC_0_MIN 500
#endif

#ifndef IR_NEC_1_MAX
#define IR_NEC_1_MAX 3000
#endif

#ifndef IR_NEC_1_MIN
#define IR_NEC_1_MIN 1800
#endif

/**
 * @brief Parse the captured PWM value. This function SHOULD be called in the PWM capture interrupt handler.
 * @param val The captured PWM value, in us.
 */
void IR_Parse(uint32_t val);

/**
 * @brief Get the parsed IR data
 * @return The parsed 8 bits IR data, or 0 if no valid data is available.
 */
uint8_t IR_GetParsedData(void);

#endif // __IR_NEC_PARSER_H__
