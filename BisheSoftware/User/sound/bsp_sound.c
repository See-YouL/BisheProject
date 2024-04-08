/**
 * @file bsp_sound.c
 * @author Eric(jff517680@gmail.com)
 * @brief 蜂鸣器驱动
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "bsp_sound.h"

/**
 * @brief 蜂鸣器的初始化配置
 * 
 * PA3配置为推挽输出模式接蜂鸣器低电平引脚
 */
void Sound_GPIO_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(SOUND_GPIO_CLK, ENABLE);

    // 配置PA3为推挽输出模式，并初始化为高电平
    GPIO_InitStructure.GPIO_Pin = SOUND_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SOUND_GPIO_PORT, &GPIO_InitStructure);

    // 将PA3引脚初始化为高电平状态
    GPIO_SetBits(SOUND_GPIO_PORT, SOUND_GPIO_PIN);
}

/**
 * @brief 蜂鸣器控制函数
 * 
 * @param state: 蜂鸣器状态
 *     @arg Sound_Open: 打开蜂鸣器
 *     @arg Sound_Close: 关闭蜂鸣器
 * @retval None
 */
void Sound_Controller(Sound_State state)
{
    if(Sound_Open == state)
    {
        GPIO_ResetBits(SOUND_GPIO_PORT, SOUND_GPIO_PIN);
    }

    if (Sound_Close == state)
    {
        GPIO_SetBits(SOUND_GPIO_PORT, SOUND_GPIO_PIN);
    }
}
