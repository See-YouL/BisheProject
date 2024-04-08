/**
 * @file bsp_mq.h
 * @author Eirc(jff517680@gmail.com)
 * @brief MQ-2
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __BSP_MQ_H
#define __BSP_MQ_H

#include "stm32f10x.h"

#define MQ2_INT_GPIO_PORT         GPIOA
#define MQ2_INT_GPIO_CLK          RCC_APB2Periph_GPIOA
#define MQ2_INT_GPIO_PIN          GPIO_Pin_4
#define MQ2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define MQ2_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define MQ2_INT_EXTI_LINE         EXTI_Line4
#define MQ2_INT_EXTI_IRQ          EXTI4_IRQn
#define MQ2_IRQHandler            EXTI4_IRQHandler

typedef enum {
    MQ2_Warn = 0,
    MQ2_Normal = 1
}MQ2_State;

void MQ2_Config(void);
MQ2_State MQ2_Read(void);

#endif // __BSP_MQ_H