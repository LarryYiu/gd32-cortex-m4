#include "exti.h"
#include "gd32f30x.h"
#include "led_driver.h"

/* Code adapted from D. Ye */

void EXTI_Config(void)
{
    /*使能GPIO的时钟*/
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOG);
    /*配置按键的IO为浮空输入模式*/
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_0);
    gpio_init(GPIOG, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_13);

    /* 使能EXTI时钟 */
    rcu_periph_clock_enable(RCU_AF);
    /* 配置I/O连接到EXTI线 */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);
    /* 配置上升/下降沿 */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    /* 清除标志 */
    exti_interrupt_flag_clear(EXTI_0);
    /* 使能中断 */
    nvic_irq_enable(EXTI0_IRQn, 1, 1);

    /* 配置I/O连接到EXTI线 */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOG, GPIO_PIN_SOURCE_13);
    /* 配置上升/下降沿 */
    exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    /* 清除标志 */
    exti_interrupt_flag_clear(EXTI_13);
    /* 使能中断 */
    nvic_irq_enable(EXTI10_15_IRQn, 0, 1);
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
    LED_Toggle(0);
}
