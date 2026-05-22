#include "timer.h"
#include <stdio.h>
#include "gd32f30x.h"
#include "led_driver.h"
#include "dwt_delay.h"

static uint32_t timer5Count = 0;

static void __GPIO_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_8);
}

static void __TIMER_Config(void)
{
    timer_parameter_struct timer_initpara;
    /* TIMER5 configuration */
    rcu_periph_clock_enable(RCU_TIMER5);
    timer_deinit(TIMER5);
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 239;
    timer_initpara.period    = 499;
    timer_init(TIMER5, &timer_initpara);
    timer_interrupt_enable(TIMER5, TIMER_INT_UP);
    nvic_irq_enable(TIMER5_IRQn, 0, 0);

    /* TIMER0 configuration */
    rcu_periph_clock_enable(RCU_TIMER0);
    timer_deinit(TIMER0);
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 119;
    timer_initpara.period    = 499;
    timer_init(TIMER0, &timer_initpara);

    /* CH0 configuration in PWM mode */
    timer_oc_parameter_struct timer_ocintpara;
    timer_channel_output_struct_para_init(&timer_ocintpara);
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 0);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_primary_output_config(TIMER0, ENABLE);

    timer_enable(TIMER0);
    timer_enable(TIMER5);
}

void TIMER_Init(void)
{
    __GPIO_Config();
    __TIMER_Config();
}

// void TIMER0_UP_IRQHandler(void)
// {
//     if(timer_interrupt_flag_get(TIMER0, TIMER_INT_FLAG_UP) != RESET)
//     {
//         timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_UP);
//         LED_Toggle(1);
//     }
// }

void TIMER5_IRQHandler(void)
{
    if(timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP) != RESET)
    {
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
        timer5Count++;
    }
}

uint32_t TIMER5_GetCount(void)
{
    return timer5Count;
}

void TIMER5_Clear(void)
{
    timer5Count = 0;
}

void PWM_Test(void)
{
    static int16_t pw         = 0;
    static bool direction     = true;
    static uint32_t lastCount = 0;
    if(timer5Count - lastCount >= 40)
    {
        lastCount = timer5Count;
        pw        = direction ? pw + 10 : pw - 10;
        if(pw >= 500 || pw <= 0)
        {
            direction = !direction;
        }
        timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, pw);
    }
}
