#include "pwm_capture.h"
#include "gd32f30x.h"
#include "ir_nec_parser.h"

static void __GPIO_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_6);
}
static void __TIMER_Config(void)
{
    /* TIMER7 configuration */
    rcu_periph_clock_enable(RCU_TIMER7);
    timer_deinit(TIMER7);

    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);

    timer_initpara.prescaler = 119;
    timer_initpara.period    = 0xffff;
    timer_init(TIMER7, &timer_initpara);

    /* TIMER7 input capture configuration */
    timer_ic_parameter_struct icInitPara;
    timer_channel_input_struct_para_init(&icInitPara);
    icInitPara.icpolarity = TIMER_IC_POLARITY_FALLING; // capture on falling edge
    timer_input_capture_config(TIMER7, TIMER_CH_0, &icInitPara);

    timer_interrupt_enable(TIMER7, TIMER_INT_CH0);
    nvic_irq_enable(TIMER7_Channel_IRQn, 0, 0);
    timer_enable(TIMER7);
}

void PWM_CAP_Init(void)
{
    __GPIO_Config();
    __TIMER_Config();
}

void TIMER7_Channel_IRQHandler(void)
{
    uint32_t icValue;
    if(timer_interrupt_flag_get(TIMER7, TIMER_INT_FLAG_CH0) == SET)
    {
        timer_interrupt_flag_clear(TIMER7, TIMER_INT_FLAG_CH0);
        icValue = timer_channel_capture_value_register_read(TIMER7, TIMER_CH_0) + 1;
        /* plus 1 to avoid the capture value is 0 when the signal duty cycle is 100% */
        timer_counter_value_config(TIMER7, 0);
        IR_Parse(icValue);
    }
}
