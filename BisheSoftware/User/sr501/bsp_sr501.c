/**
 * @file bsp_sr501.c
 * @author Eric(jff517680@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "bsp_sr501.h"

// /**
//  * @brief  配置嵌套向量中断控制器NVIC
//  * @param  无
//  * @retval 无
//  */
// static void NVIC_Configuration(void)
// {
//     NVIC_InitTypeDef NVIC_InitStructure;

//     /* 配置NVIC为优先级组2 */
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//     /* 配置中断源：按键1 */
//     NVIC_InitStructure.NVIC_IRQChannel = SR501_INT_EXTI_IRQ;
//     /* 配置抢占优先级 */
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//     /* 配置子优先级 */
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//     /* 使能中断通道 */
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
// }

/**
 * @brief  配置 IO为输入口
 * @param  无
 * @retval 无
 */
void EXTI_SR501_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启GPIO口的时钟*/
    RCC_APB2PeriphClockCmd(SR501_INT_GPIO_CLK, ENABLE);

    /*--------------------------GPIO配置-----------------------------*/
    /* 选择用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = SR501_INT_GPIO_PIN;
    /* 配置为下拉输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(SR501_INT_GPIO_PORT, &GPIO_InitStructure);

}
/*********************************************END OF FILE**********************/
