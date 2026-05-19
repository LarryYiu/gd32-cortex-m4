#include "uart.h"
#include "led_driver.h"

#define USART0_DATA_ADDR (USART0 + 0x04U)
#define PACKET_DATA_LEN (6)
static uint8_t g_rcvDataBuf[PACKET_DATA_LEN];
static bool g_pktRcvd;

void UART_Config(uint32_t baudrate)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GPIO_PIN_10);

    /* 使能UART时钟；*/
    rcu_periph_clock_enable(RCU_USART0);
    /* 复位UART；*/
    usart_deinit(USART0);

    /* 通过USART_CTL0寄存器的WL设置字长；*/
    /* 通过USART_CTL0寄存器的PCEN设置校验位；*/
    /* 在USART_CTL1寄存器中写STB[1:0]位来设置停止位的长度；*/
    /* 以上保持默认，8位数据位，1位停止位，没有奇偶校验位 */

    /* 在USART_BAUD寄存器中设置波特率；*/
    usart_baudrate_set(USART0, baudrate);
    /* 在USART_CTL0寄存器中设置TEN位，使能发送功能；*/
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    /* 在USART_CTL0寄存器中设置REN位，使能接收功能；*/
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    /* 打开串口模块中的中断开关 */
    // usart_interrupt_enable(USART0, USART_INT_RBNE);  // 接收非空中断
    usart_interrupt_enable(USART0, USART_INT_IDLE); // 接收空闲中断
    /* 打开串口dma接收数据功能 */
    usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
    /* 打开NVIC中的中断开关 */
    nvic_irq_enable(USART0_IRQn, 0, 0);
    /* 在USART_CTL0寄存器中置位UEN位，使能UART；*/
    usart_enable(USART0);

    /* 使能DMA时钟；*/
    rcu_periph_clock_enable(RCU_DMA0);
    /* 复位DMA通道；*/
    dma_deinit(DMA0, DMA_CH4);

    dma_parameter_struct dmaStruct;
    dma_struct_para_init(&dmaStruct);
    /* 配置传输方向；*/
    dmaStruct.direction = DMA_PERIPHERAL_TO_MEMORY;
    /* 配置数据源地址；*/
    dmaStruct.periph_addr = USART0_DATA_ADDR;
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
    dma_init(DMA0, DMA_CH4, &dmaStruct);
    /* 使能DMA通道；*/
    dma_channel_enable(DMA0, DMA_CH4);
}

/**
***********************************************************************
包格式：帧头H    帧头L    功能字   LED编号      亮/灭     XOR_Checksum
        0x55    0xAA     0x06      0x02       0x01       0x--
***********************************************************************
*/
#define FRAME_HEADER_H (0x55)
#define FRAME_HEADER_L (0xAA)
#define FUNC_CODE_LED_WRITE (0x06)
#define FUNC_CODE_IDX (2) // 功能字数组下标
#define LED_ID_IDX (3)    // LED编号数组下标
#define LED_CMD_IDX (4)   // 命令字数组下标

static uint8_t XorCheckSum(uint8_t* data, uint8_t len)
{
    uint8_t checksum = 0;
    for(uint8_t i = 0; i < len; i++)
    {
        checksum ^= data[i];
    }
    return checksum;
}

// static void UART_Process(uint8_t data)
// {
//     static uint8_t s_lenRecv  = 0;
//     g_rcvDataBuf[s_lenRecv++] = data;
//     switch(s_lenRecv)
//     {
//         case 1:
//             if(g_rcvDataBuf[0] != FRAME_HEADER_H)
//             {
//                 s_lenRecv = 0;
//             }
//             return;
//         case 2:
//             if(g_rcvDataBuf[1] != FRAME_HEADER_L)
//             {
//                 s_lenRecv = 0;
//             }
//             return;
//         case PACKET_DATA_LEN:
//             s_lenRecv = 0;
//             g_pktRcvd = true;
//             return;
//         default:
//             return;
//     }
// }

void USART0_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) != RESET)
    {
        usart_data_receive(USART0);
        if(PACKET_DATA_LEN == (PACKET_DATA_LEN - dma_transfer_number_get(DMA0, DMA_CH4)))
        {
            g_pktRcvd = true;
        }
        dma_channel_disable(DMA0, DMA_CH4);
        dma_transfer_number_config(DMA0, DMA_CH4, PACKET_DATA_LEN);
        dma_channel_enable(DMA0, DMA_CH4);
    }
}

void UART0_Task()
{
    if(!g_pktRcvd)
    {
        return;
    }
    g_pktRcvd = false;
    if(g_rcvDataBuf[0] != FRAME_HEADER_H || g_rcvDataBuf[1] != FRAME_HEADER_L)
    {
        return;
    }
    else if(XorCheckSum(g_rcvDataBuf, PACKET_DATA_LEN - 1) != g_rcvDataBuf[PACKET_DATA_LEN - 1])
    {
        return;
    }
    else
    {
        for(uint8_t i = 0; i < PACKET_DATA_LEN; i++)
        {
            printf("%02X ", g_rcvDataBuf[i]);
        }
        printf("\r\n");

        if(g_rcvDataBuf[FUNC_CODE_IDX] == FUNC_CODE_LED_WRITE)
        {
            LED_SetState(g_rcvDataBuf[LED_ID_IDX], g_rcvDataBuf[LED_CMD_IDX] ? SET : RESET);
        }
    }
}

int fputc(int ch, FILE* f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
    // add timeout here to avoid blocking, not necessary
    return ch;
}
