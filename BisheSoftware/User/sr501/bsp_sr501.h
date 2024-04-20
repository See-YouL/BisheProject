/**
 * @file bsp_sr501.h
 * @author Eric(jff517680@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/**
 * @brief 引脚情况
 * 
 * 1. VCC -> 5V
 * 2. GND -> GND
 * 3. OUT -> PA8
 */
#ifndef __BSP_SR501_H
#define	__BSP_SR501_H

#include "stm32f10x.h"

#define SR501_INT_GPIO_PORT         GPIOA
#define SR501_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define SR501_INT_GPIO_PIN          GPIO_Pin_8
#define SR501_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define SR501_INT_EXTI_PINSOURCE    GPIO_PinSource8
#define SR501_INT_EXTI_LINE         EXTI_Line8
#define SR501_INT_EXTI_IRQ          EXTI9_5_IRQn

#define SR501_IRQHandler            EXTI9_5_IRQHandler


void EXTI_SR501_Config(void);


#endif // __BSP_SR501_H
