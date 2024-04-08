/**
 * @file bsp_mq.c
 * @author Eirc(jff517680@gmail.com)
 * @brief MQ-2
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "bsp_mq.h"

/**
 * @brief 配置NVIC
 * 抢占优先级0 响应优先级0
 */
static void MQ2_NVIC_Config(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    // 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = MQ2_INT_EXTI_IRQ; // 外部中断4
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 配置GPIO 
 * PA4配置为输入模式
 */
static void MQ2_GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(MQ2_INT_GPIO_CLK, ENABLE);

    // 配置PA4为输入模式
    GPIO_InitStructure.GPIO_Pin = MQ2_INT_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MQ2_INT_GPIO_PORT, &GPIO_InitStructure);
}


/**
 * @brief 配置外部中断线
 * 配置PA4引脚为外部中断线
 */
static void MQ2_EXTI_Config(void) 
{
    EXTI_InitTypeDef EXTI_InitStructure;

    // 使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 配置PA4引脚为外部中断线4
    GPIO_EXTILineConfig(MQ2_INT_EXTI_PORTSOURCE, MQ2_INT_EXTI_PINSOURCE );

    // 配置外部中断线4
    EXTI_InitStructure.EXTI_Line = MQ2_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/**
 * @brief MQ-2初始化配置
 * 
 */
void MQ2_Config(void)
{
    MQ2_GPIO_Config();
    MQ2_EXTI_Config();
    MQ2_NVIC_Config();
}

/**
 * @brief MQ-2检测有害气体 
 * 
 * @retval 检测到的有害气体状态
 * - MQ2_Warn: 检测到有害气体
 * - MQ2_Normal: 未检测到有害气体
 */
MQ2_State MQ2_Read(void)
{
    if(0 == GPIO_ReadInputDataBit(MQ2_INT_GPIO_PORT, MQ2_INT_GPIO_PIN))
    {
        return MQ2_Warn;
    }
    else
    {
        return MQ2_Normal;
    }
}