#include <stdbool.h>
#include <stdio.h>
#include "wifi_driver.h"
#include "gd32f30x.h"

#define USART2_DATA_ADDR (USART2 + 0x04U) // USART_DATA register address
#define PACKET_DATA_LEN (256)
static uint8_t g_rcvDataBuf[PACKET_DATA_LEN];
static bool g_pktRcvd;
static uint32_t g_dataLen;

void WIFI_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOG);
    gpio_init(GPIOG, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_7);
    gpio_bit_set(GPIOG, GPIO_PIN_7); // set PG7 high to power on the WiFi module

    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GPIO_PIN_11);

    /* 使能UART时钟；*/
    rcu_periph_clock_enable(RCU_USART2);
    /* 复位UART；*/
    usart_deinit(USART2);

    /* 通过USART_CTL0寄存器的WL设置字长；*/
    /* 通过USART_CTL0寄存器的PCEN设置校验位；*/
    /* 在USART_CTL1寄存器中写STB[1:0]位来设置停止位的长度；*/
    /* 以上保持默认，8位数据位，1位停止位，没有奇偶校验位 */

    /* 在USART_BAUD寄存器中设置波特率；*/
    usart_baudrate_set(USART2, 115200U);
    /* 在USART_CTL0寄存器中设置TEN位，使能发送功能；*/
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    /* 在USART_CTL0寄存器中设置REN位，使能接收功能；*/
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    /* 打开串口模块中的中断开关 */
    // usart_interrupt_enable(USART2, USART_INT_RBNE);  // 接收非空中断
    usart_interrupt_enable(USART2, USART_INT_IDLE); // 接收空闲中断
    /* 打开串口dma接收数据功能 */
    usart_dma_receive_config(USART2, USART_RECEIVE_DMA_ENABLE);
    /* 打开NVIC中的中断开关 */
    nvic_irq_enable(USART2_IRQn, 0, 0);
    /* 在USART_CTL0寄存器中置位UEN位，使能UART；*/
    usart_enable(USART2);

    /* 使能DMA时钟；*/
    rcu_periph_clock_enable(RCU_DMA0);
    /* 复位DMA通道；*/
    dma_deinit(DMA0, DMA_CH2);

    dma_parameter_struct dmaStruct;
    dma_struct_para_init(&dmaStruct);
    /* 配置传输方向；*/
    dmaStruct.direction = DMA_PERIPHERAL_TO_MEMORY;
    /* 配置数据源地址；*/
    dmaStruct.periph_addr = USART2_DATA_ADDR;
    /* 配置源地址是固定的还是增长的；*/
    dmaStruct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    /* 配置源数据传输位宽；*/
    dmaStruct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;

    /* 配置数据目的地址；*/
    dmaStruct.memory_addr = (uint32_t)g_rcvDataBuf;
    /* 配置目的地址是固定的还是增长的；*/
    dmaStruct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    /* 配置目的数据传输位宽；*/
    dmaStruct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    /* 配置数据传输最大次数；*/
    dmaStruct.number = PACKET_DATA_LEN;
    /* 配置DMA通道优先级；*/
    dmaStruct.priority = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH2, &dmaStruct);
    /* 使能DMA通道；*/
    dma_channel_enable(DMA0, DMA_CH2);
}

void WIFI_SendData(uint8_t* data, uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
        usart_data_transmit(USART2, data[i]);
    }
}

void USART2_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE) != RESET)
    {
        g_dataLen = PACKET_DATA_LEN - dma_transfer_number_get(DMA0, DMA_CH2);
        usart_data_receive(USART2);
        g_pktRcvd = true;
        dma_channel_disable(DMA0, DMA_CH2);
        dma_transfer_number_config(DMA0, DMA_CH2, PACKET_DATA_LEN);
        dma_channel_enable(DMA0, DMA_CH2);
    }
}

void WIFI_Task(void)
{
    if(g_pktRcvd)
    {
        g_pktRcvd = false;

        printf("Packet len: %d\r\n", g_dataLen);
        // for(uint8_t i = 8; i < g_dataLen; i++)
        // {
        //     printf("%02X ", g_rcvDataBuf[i]);
        // }
        // printf("\r\n");
        g_rcvDataBuf[g_dataLen] = '\0'; // null terminate the received data
        printf("%s\r\n", g_rcvDataBuf);
    }
}
