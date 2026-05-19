#include <stdbool.h>
#include <stdio.h>
#include "wifi_driver.h"
#include "gd32f30x.h"

#define USART2_DATA_ADDR (USART2 + 0x04U) // USART_DATA register address
#define PACKET_DATA_LEN (10)
static uint8_t g_rcvDataBuf[PACKET_DATA_LEN];
static bool g_pktRcvd;

void WIFI_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOG);
    gpio_init(GPIOG, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_7);
    gpio_bit_set(GPIOG, GPIO_PIN_7); // set PG7 high to power on the WiFi module

    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GPIO_PIN_11);

    rcu_periph_clock_enable(RCU_USART2);
    usart_deinit(USART2);
    usart_baudrate_set(USART2, 115200U);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_interrupt_enable(USART2, USART_INT_RBNE); // enable data register interrupt
    nvic_irq_enable(USART2_IRQn, 0, 0);
    usart_enable(USART2);

    // rcu_periph_clock_enable(RCU_DMA0);
    // dma_deinit(DMA0, DMA_CH2);
    // dma_parameter_struct dmaStruct;
    // dma_struct_para_init(&dmaStruct);
    // dmaStruct.direction    = DMA_PERIPHERAL_TO_MEMORY;
    // dmaStruct.periph_addr  = USART2_DATA_ADDR;
    // dmaStruct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    // dmaStruct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    // dmaStruct.memory_addr  = (uint32_t)g_rcvDataBuf;
    // dmaStruct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    // dmaStruct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    // dmaStruct.number       = PACKET_DATA_LEN;
    // dmaStruct.priority     = DMA_PRIORITY_HIGH;
    // dma_init(DMA0, DMA_CH2, &dmaStruct);
    // dma_transfer_number_config(DMA0, DMA_CH2, PACKET_DATA_LEN);
    // dma_channel_enable(DMA0, DMA_CH2);
}

void WIFI_SendData(uint8_t* data, uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
        usart_data_transmit(USART2, data[i]);
    }
}

static void UART_Process(uint8_t data)
{
    static uint8_t s_lenRecv  = 0;
    g_rcvDataBuf[s_lenRecv++] = data;
    if(s_lenRecv >= PACKET_DATA_LEN)
    {
        s_lenRecv = 0;
        g_pktRcvd = true;
    }
}

void USART2_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE) != RESET)
    {
        uint8_t uData = usart_data_receive(USART2);
        UART_Process(uData);
    }
}

void WIFI_Task(void)
{
    if(g_pktRcvd)
    {
        g_pktRcvd = false;
        printf("%s\r\n", g_rcvDataBuf);
    }
}
