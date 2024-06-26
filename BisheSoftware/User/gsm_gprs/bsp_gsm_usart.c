/**
 ******************************************************************************
 * @file    bsp_gsm_usart.c
 * @author  fire
 * @version V1.0
 * @date    2013-xx-xx
 * @brief   GSM模块的串口驱动
 ******************************************************************************
 * @attention
 *
 * 实验平台:野火 F103-霸道 STM32 开发板
 * 论坛    :http://www.firebbs.cn
 * 淘宝    :https://fire-stm32.taobao.com
 *
 ******************************************************************************
 */
#include "./gsm_gprs/bsp_gsm_usart.h"
#include <stdarg.h>

/// 配置USART接收中断
static void GSM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = GSM_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：GSM_USART_Config
 * 描述  ：GSM_USART GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void GSM_USART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* config GSM_USARTx clock */
    RCC_APB2PeriphClockCmd(GSM_USART_GPIO_CLK, ENABLE);
    GSM_USART_APBxClkCmd(GSM_USART_CLK, ENABLE);

    /* USART GPIO config */
    /* Configure USART Tx  as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GSM_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GSM_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin  = GSM_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GSM_RX_GPIO_PORT, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate            = GSM_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(GSM_USARTx, &USART_InitStructure);

    /*	配置中断优先级 */
    GSM_NVIC_Config();
    /* 使能串口2接收中断 */
    USART_ITConfig(GSM_USARTx, USART_IT_RXNE, ENABLE);

    USART_Cmd(GSM_USARTx, ENABLE);
}

/**
 * @brief GSM初始化测试 
 * 
 */
void GSM_Init(void){
    uint8_t testCard = 0; // GSM Init
    while (gsm_init() != GSM_TRUE) {
        printf("ERROR: GSM module not initialized \n");
        printf("The GSM module response test is not working properly \n");
        printf("Please check whether the module is connected or whether the power switch is turned on \n");
        GSM_DELAY(500);
    }
    printf("Passed the GSM module response test \n");
    GSM_DELAY(1000);

    // Set the text mode
    if (gsm_cmd("AT+CMGF=1\r", "OK", 100) != GSM_TRUE) {
        printf("ERROR: Text mode setting \n");
    }
    GSM_DELAY(1000);

    printf("Waiting for GSM module initializationa \n");
    while (IsInsertCard() != GSM_TRUE) {
        if (++testCard > 20) {
            printf("ERROR: Phone card not detcted \n");
            printf("If the phone card is not detected, please power off and reconnect the phone card \n");
        }
        GSM_DELAY(1000);
    }
    printf("GSM Init Completed \n");
    GSM_DELAY(1000);
}

void GSM_Dialup(void)
{
    char *rebuff; // 拨号使用
    printf("Dial-up test after 5 seconds...\n");
    GSM_DELAY(5000);                     // Delay
    gsm_call("13704161756");             // Call the number
    rebuff = gsm_waitask(0);             // Receive a response
    if (strstr(rebuff, "ATD") != NULL) { // The response is OK
        printf("Calling now \n");
        GSM_CLEAN_RX();          // Clear the receive buffer
        rebuff = gsm_waitask(0); // Wait for it to disappear again

        if (strstr(rebuff, "NO CARRIER") != NULL) { // Respond NO CARRIER, End Call
            printf("End Call \n");
        } else if (strstr(rebuff, "NO ANSWER") != NULL) { // Pespond NO ANSWER, End Call
            printf("No Answer \n");
        }
    }
    GSM_CLEAN_RX(); // Clear the receive buffer
}

void GSM_Messeage(void)
{
    char MessageNum[] = "13704161756"; // 短信接收号码
    printf("SMS starts after 5 seconds... \n");
    GSM_DELAY(5000); // Delay 5s

    if (gsm_sms((char *)MessageNum, "Test 04-26") == GSM_TRUE) { // Send Message
        printf(":Message is already send to  %s \n", MessageNum);
    } else {
        printf("If the SMS message cannot be sent, Please confirm that the destination MessageNumber is valid \n");
    }
    GSM_DELAY(2000); // Delay 2s
}
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到GSM_USARTx
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
// int fputc(int ch, FILE *f)
//{
///* 将Printf内容发往串口 */
//  USART_SendData(GSM_USARTx, (unsigned char) ch);
//  while (!(GSM_USARTx->SR & USART_FLAG_TXE));
//
//  return (ch);
//}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被GSM_USARTx_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int i, d;
    int flag  = 0;
    char *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10) {
        *ptr = 0;
        return string;
    }

    if (!value) {
        *ptr++ = 0x30;
        *ptr   = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0) {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10) {
        d = value / i;

        if (d || flag) {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

#if 1
// 中断缓存串口数据
#define UART_BUFF_SIZE 255
volatile uint8_t uart_p = 0;
uint8_t uart_buff[UART_BUFF_SIZE];

void bsp_GSM_USART_IRQHandler(void)
{
    if (uart_p < UART_BUFF_SIZE) {
        if (USART_GetITStatus(GSM_USARTx, USART_IT_RXNE) != RESET) {
            uart_buff[uart_p] = USART_ReceiveData(GSM_USARTx);
            uart_p++;
        }
    }
}

// 获取接收到的数据和长度
char *get_rebuff(uint8_t *len)
{
    *len = uart_p;
    return (char *)&uart_buff;
}

void clean_rebuff(void)
{
    uint16_t i = UART_BUFF_SIZE + 1;
    uart_p     = 0;
    while (i)
        uart_buff[--i] = 0;
}

#endif

/*
 * 函数名：GSM_USARTx_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即GSM_USARTx
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 *         典型应用GSM_USARTx_printf( GSM_USARTx, "\r\n this is a demo \r\n" );
 *            		 GSM_USARTx_printf( GSM_USARTx, "\r\n %d \r\n", i );
 *            		 GSM_USARTx_printf( GSM_USARTx, "\r\n %s \r\n", j );
 */
void GSM_USART_printf(char *Data, ...)
{
    const char *s;
    int d;
    char buf[16];

    va_list ap;
    va_start(ap, Data);

    while (*Data != 0) // 判断是否到达字符串结束符
    {
        if (*Data == 0x5c) //'\'
        {
            switch (*++Data) {
                case 'r': // 回车符
                    USART_SendData(GSM_USARTx, 0x0d);
                    Data++;
                    break;

                case 'n': // 换行符
                    USART_SendData(GSM_USARTx, 0x0a);
                    Data++;
                    break;

                default:
                    Data++;
                    break;
            }
        } else if (*Data == '%') { //
            switch (*++Data) {
                case 's': // 字符串
                    s = va_arg(ap, const char *);
                    for (; *s; s++) {
                        USART_SendData(GSM_USARTx, *s);
                        while (USART_GetFlagStatus(GSM_USARTx, USART_FLAG_TXE) == RESET)
                            ;
                    }
                    Data++;
                    break;

                case 'd': // 十进制
                    d = va_arg(ap, int);
                    itoa(d, buf, 10);
                    for (s = buf; *s; s++) {
                        USART_SendData(GSM_USARTx, *s);
                        while (USART_GetFlagStatus(GSM_USARTx, USART_FLAG_TXE) == RESET)
                            ;
                    }
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }
        } /* end of else if */
        else
            USART_SendData(GSM_USARTx, *Data++);
        while (USART_GetFlagStatus(GSM_USARTx, USART_FLAG_TXE) == RESET)
            ;
    }
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
