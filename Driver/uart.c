#include "uart.h"
#include "led_driver.h"

void UART_Config(uint32_t baudrate)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GPIO_PIN_10);

    rcu_periph_clock_enable(RCU_USART0);
    usart_deinit(USART0);
    usart_baudrate_set(USART0, baudrate);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_interrupt_enable(USART0, USART_INT_RBNE); // enable data register interrupt
    nvic_irq_enable(USART0_IRQn, 0, 0);
    usart_enable(USART0);
}

#define PACKAGE_DATA_LEN (6)
static uint8_t g_rcvDataBuf[PACKAGE_DATA_LEN];
static bool g_pktRcvd;
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

static void UART_Process(uint8_t data)
{
    static uint8_t s_lenRecv  = 0;
    g_rcvDataBuf[s_lenRecv++] = data;
    switch(s_lenRecv)
    {
        case 1:
            if(g_rcvDataBuf[0] != FRAME_HEADER_H)
            {
                printf("1 0x%02X \r\n", g_rcvDataBuf[0]);
                s_lenRecv = 0;
            }
            return;
        case 2:
            if(g_rcvDataBuf[1] != FRAME_HEADER_L)
            {
                printf("2 0x%02X \r\n", g_rcvDataBuf[1]);
                s_lenRecv = 0;
            }
            return;
        case PACKAGE_DATA_LEN:
            printf("received\r\n");
            s_lenRecv = 0;
            g_pktRcvd = true;
            return;
        default:
            return;
    }
}

void USART0_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) == SET)
    {
        // usart_interrupt_flag_clear(USART0, USART_INT_RBNE);
        uint8_t uData = usart_data_receive(USART0); // this will clear the RBNE flag
    }
}

void UART0_Task()
{
    // printf("in task \r\n");
    if(!g_pktRcvd)
    {
        return;
    }
    else
    {
        printf("processing \r\n");
        if(XorCheckSum(g_rcvDataBuf, PACKAGE_DATA_LEN - 1) != g_rcvDataBuf[PACKAGE_DATA_LEN - 1])
        {
            return; // checksum error, discard the packet
        }
        g_pktRcvd = false;
        for(uint8_t i = 0; i < 6; i++)
        {
            printf("0x%02X ", g_rcvDataBuf[i]);
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
