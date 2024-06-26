/**
 * @file bsp_gsm_usart.h
 * @author Eric(jff517680@gmail.com)
 * @brief GSM模块定义
 * @version 0.1
 * @date 2024-04-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/**
 * @brief 引脚情况
 * 
 * 1. VCC -> 外部供电
 * 2. GND -> GND
 * 3. SRX -> PA2
 * 4. STX -> PA3
 */

#ifndef __BSP_GSM_USART_H
#define	__BSP_GSM_USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_gsm_gprs.h"


// GSM_GPRS使用的串口
#define  GSM_USARTx                   USART2
#define  GSM_USART_CLK                RCC_APB1Periph_USART2
#define  GSM_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  GSM_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  GSM_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
    
#define  GSM_TX_GPIO_PORT         GPIOA   
#define  GSM_TX_GPIO_PIN          GPIO_Pin_2
#define  GSM_RX_GPIO_PORT       GPIOA
#define  GSM_RX_GPIO_PIN        GPIO_Pin_3

#define  GSM_USART_IRQ                USART2_IRQn
#define  GSM_USART_IRQHandler         USART2_IRQHandler

void GSM_USART_Config(void);
void GSM_Init(void);
void GSM_Dialup(void);
void GSM_Messeage(void);
void bsp_GSM_USART_IRQHandler(void);
char *get_rebuff(uint8_t *len);
void clean_rebuff(void);

void GSM_USART_printf(char *Data,...);

#endif /* __BSP_GSM_USART_H */
