#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "gd32f30x.h"
// clang-format off

/*LED CONFIGURATION START*/

#ifndef NULL
#define NULL ((void*)0)
#endif

#define LED_GPIO_LOOKUP \
    GPIO_PIN(F,6), \
    GPIO_PIN(E,6), \
    GPIO_PIN(A,8)

#define LED_GPIO_FREQENCY GPIO_OSPEED_2MHZ

/*LED CONFIGURATION END*/

/*KEY CONFIGURATION START*/

#define KEY_LOOK_UP \
    GPIO_PIN(A,0), \
    GPIO_PIN(G,13), \
    GPIO_PIN(G,14), \
    GPIO_PIN(G,15)

#define KEY_GPIO_FREQENCY GPIO_OSPEED_2MHZ
#define KEY_RELEASE_TRIGGER_DEFAULT true
#define KEY_LONG_PRESS_THRESHOLD 1000
#define KEY_CONTINUOUS_PRESS_THRESHOLD 200
#define KEY_DEBOUNCE_TIME_MS 10

/*KEY CONFIGURATION END*/

// clang-format on
#define len(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif // __CONFIG_H__
