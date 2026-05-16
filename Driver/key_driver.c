#include "key_driver.h"

#ifndef KEY_USE_BUSY_WAIT
#define KEY_USE_BUSY_WAIT true
#endif

#ifndef KEY_GPIO_FREQENCY
#define KEY_GPIO_FREQENCY GPIO_OSPEED_2MHZ
#endif

#ifndef KEY_RELEASE_TRIGGER_DEFAULT
#define KEY_RELEASE_TRIGGER_DEFAULT true
#endif

#ifndef KEY_LONG_PRESS_THRESHOLD
#define KEY_LONG_PRESS_THRESHOLD 5
#endif

#ifndef KEY_CONTINUOUS_PRESS_THRESHOLD
#define KEY_CONTINUOUS_PRESS_THRESHOLD 3
#endif

#define KEY_NUM len(__KEY_IDEN_LOOKUP)

struct Key
{
    uint8_t gpioIden;
    uint8_t pressDuration;
    uint8_t pressCount;
    uint8_t lastReleaseDuration;
    bool isPressing;
    bool isReleaseTriggered;
    void (*onShortPress)(void);
    void (*onLongPress)(void);
};
typedef struct Key Key_t;

static const uint8_t __KEY_IDEN_LOOKUP[] = {KEY_LOOK_UP};
static Key_t __KEYS[KEY_NUM];

void KEY_Config(void)
{
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        rcu_periph_clock_enable(GPIO_GetRcuPeriph(__KEY_IDEN_LOOKUP[i]));
        gpio_init(GPIO_GetPeriphAddr(__KEY_IDEN_LOOKUP[i]), GPIO_MODE_IN_FLOATING, KEY_GPIO_FREQENCY,
                  GPIO_GetPinAddr(__KEY_IDEN_LOOKUP[i]));

        __KEYS[i].gpioIden            = __KEY_IDEN_LOOKUP[i];
        __KEYS[i].pressDuration       = 0;
        __KEYS[i].pressCount          = 0;
        __KEYS[i].lastReleaseDuration = 0;
        __KEYS[i].isPressing          = false;
        __KEYS[i].isReleaseTriggered  = true;
        __KEYS[i].onShortPress        = NULL;
        __KEYS[i].onLongPress         = NULL;
    }
}

void KEY_Enable(void)
{
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        gpio_init(GPIO_GetPeriphAddr(__KEY_IDEN_LOOKUP[i]), GPIO_MODE_IN_FLOATING, KEY_GPIO_FREQENCY,
                  GPIO_GetPinAddr(__KEY_IDEN_LOOKUP[i]));
    }
}
void KEY_Disable(void)
{
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        gpio_init(GPIO_GetPeriphAddr(__KEY_IDEN_LOOKUP[i]), GPIO_MODE_AIN, KEY_GPIO_FREQENCY,
                  GPIO_GetPinAddr(__KEY_IDEN_LOOKUP[i]));
    }
}

void KEY_AddShortPressListener(uint8_t keyIndex, void (*listener)(void))
{
    if(keyIndex < KEY_NUM)
    {
        __KEYS[keyIndex].onShortPress = listener;
    }
}

void KEY_AddLongPressListener(uint8_t keyIndex, void (*listener)(void))
{
    if(keyIndex < KEY_NUM)
    {
        __KEYS[keyIndex].onLongPress = listener;
    }
}
void KEY_RemoveShortPressListener(uint8_t keyIndex)
{
    if(keyIndex < KEY_NUM)
    {
        __KEYS[keyIndex].onShortPress = NULL;
    }
}
void KEY_RemoveLongPressListener(uint8_t keyIndex)
{
    if(keyIndex < KEY_NUM)
    {
        __KEYS[keyIndex].onLongPress = NULL;
    }
}
