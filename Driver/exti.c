#include "exti.h"
#include "gd32f30x.h"

void EXTI_Config(uint8_t encodedGPIO, exti_mode_enum extiMode, exti_trig_type_enum triggerType, uint8_t prePriority,
                 uint8_t subPriority)
{
    uint8_t gpioPin            = encodedGPIO & 0x0F;
    exti_line_enum extiLineBit = (exti_line_enum)BIT(gpioPin);
    rcu_periph_clock_enable(RCU_AF);
    gpio_exti_source_select(encodedGPIO >> 4, gpioPin);
    exti_init(extiLineBit, extiMode, triggerType);
    exti_interrupt_flag_clear(extiLineBit);
    if(gpioPin <= 4)
    {
        nvic_irq_enable((IRQn_Type)(EXTI0_IRQn + gpioPin), prePriority, subPriority);
    }
    else if(gpioPin <= 9)
    {
        nvic_irq_enable(EXTI5_9_IRQn, prePriority, subPriority);
    }
    else
    {
        nvic_irq_enable(EXTI10_15_IRQn, prePriority, subPriority);
    }
}

void EXTI_DisableAll(void)
{
    rcu_periph_clock_disable(RCU_AF);
    exti_interrupt_disable(EXTI_0);
    exti_interrupt_disable(EXTI_1);
    exti_interrupt_disable(EXTI_2);
    exti_interrupt_disable(EXTI_3);
    exti_interrupt_disable(EXTI_4);
    exti_interrupt_disable(EXTI_5);
    exti_interrupt_disable(EXTI_6);
    exti_interrupt_disable(EXTI_7);
    exti_interrupt_disable(EXTI_8);
    exti_interrupt_disable(EXTI_9);
    exti_interrupt_disable(EXTI_10);
    exti_interrupt_disable(EXTI_11);
    exti_interrupt_disable(EXTI_12);
    exti_interrupt_disable(EXTI_13);
    exti_interrupt_disable(EXTI_14);
    exti_interrupt_disable(EXTI_15);
    nvic_irq_disable(EXTI0_IRQn);
    nvic_irq_disable(EXTI1_IRQn);
    nvic_irq_disable(EXTI2_IRQn);
    nvic_irq_disable(EXTI3_IRQn);
    nvic_irq_disable(EXTI4_IRQn);
    nvic_irq_disable(EXTI5_9_IRQn);
    nvic_irq_disable(EXTI10_15_IRQn);
}

void EXTI_Enable(exti_line_enum extiLine, uint8_t prePriority, uint8_t subPriority)
{
    exti_interrupt_enable(extiLine);
    uint8_t i = 0;
    while(extiLine >> i)
    {
        i++;
    }
    i--;
    if(i <= 4)
    {
        nvic_irq_enable((IRQn_Type)(EXTI0_IRQn + i), prePriority, subPriority);
    }
    else if(i <= 9)
    {
        nvic_irq_enable(EXTI5_9_IRQn, prePriority, subPriority);
    }
    else
    {
        nvic_irq_enable(EXTI10_15_IRQn, prePriority, subPriority);
    }
}
void EXTI_Disable(exti_line_enum extiLine)
{
    exti_interrupt_disable(extiLine);
    uint8_t i = 0;
    while(extiLine >> i)
    {
        i++;
    }
    i--;
    if(i <= 4)
    {
        nvic_irq_disable((IRQn_Type)(EXTI0_IRQn + i));
    }
    else if(i <= 9)
    {
        nvic_irq_disable(EXTI5_9_IRQn);
    }
    else
    {
        nvic_irq_disable(EXTI10_15_IRQn);
    }
}

void EXTI0_IRQHandler(void)
{
    exti_interrupt_flag_clear(EXTI_0);
    // do something
}
