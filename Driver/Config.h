#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "gd32f30x.h"
// clang-format off

/*LED CONFIGURATION START*/

#define LED_LOOK_UP \
    GPIO_Ident(A,8), \
    GPIO_Ident(E,6), \
    GPIO_Ident(F,6)

#define LED_GPIO_FREQENCY GPIO_OSPEED_2MHZ

/*LED CONFIGURATION END*/

/*KEY CONFIGURATION START*/

#define KEY_LOOK_UP \
    GPIO_Ident(A,0), \
    GPIO_Ident(G,13), \
    GPIO_Ident(G,14), \
    GPIO_Ident(G,15)

#define KEY_GPIO_FREQENCY GPIO_OSPEED_2MHZ
#define KEY_RELEASE_TRIGGER_DEFAULT true
#define KEY_LONG_PRESS_THRESHOLD 5
#define KEY_CONTINUOUS_PRESS_THRESHOLD 3

/*KEY CONFIGURATION END*/

// clang-format on
#define len(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif // __CONFIG_H__
