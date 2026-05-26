#include "adc.h"
#include <stdint.h>
#include <stdio.h>
#include "gd32f30x.h"

#define ADC0_RDATA_ADDR (ADC0 + 0x4C)
static uint16_t g_adcVal[2];

void __GPIO_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_2 | GPIO_PIN_3);
}

void __ADC_Config(void)
{
    /* Enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* Set prescaler */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4); // main 120MhZ / 4 = 30Mhz
    /* Set independent mode */
    adc_mode_config(ADC_MODE_FREE); // multiple ADCs work independently
                                    /* Scan mode: single trigger or continuous */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* Set data alignment */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* Set number of conversion channels */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);
    /* Set which channel to convert and its sequence position */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_12, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_13, ADC_SAMPLETIME_55POINT5);
    /* Set external trigger source */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /* Enable external trigger */
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    /* Enable DMA mode */
    adc_dma_mode_enable(ADC0);
    /* Enable ADC */
    adc_enable(ADC0);
    /* Internal calibration */
    adc_calibration_enable(ADC0);
    /* Start conversion by software trigger */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

void __DMA_Config(void)
{
    rcu_periph_clock_enable(RCU_DMA0);
    dma_deinit(DMA0, DMA_CH0);

    dma_parameter_struct dmaStruct;
    dma_struct_para_init(&dmaStruct);
    dmaStruct.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dmaStruct.periph_addr  = ADC0_RDATA_ADDR;
    dmaStruct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dmaStruct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dmaStruct.memory_addr  = (uint32_t)g_adcVal;
    dmaStruct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dmaStruct.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dmaStruct.number       = 2;
    dmaStruct.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dmaStruct);
    dma_circulation_enable(DMA0, DMA_CH0);
    dma_channel_enable(DMA0, DMA_CH0);
}

void ADC_Init(void)
{
    __GPIO_Config();
    __ADC_Config();
    __DMA_Config();
}

uint16_t ADC_Read()
{
    // adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // if in continuous mode, no need to trigger every time
    while(adc_flag_get(ADC0, ADC_FLAG_EOC) != SET);
    uint16_t val = adc_regular_data_read(ADC0);
    return val;
}

float ADC_GetVres()
{
    uint16_t adcVal = ADC_Read();
    return adcVal / 4095.0f * 3.3f;
}

void VresDrvTest(void)
{
    printf("CH12 adcVal = %d\n", g_adcVal[0]);
    printf("CH13 adcVal = %d\n", g_adcVal[1]);
}
