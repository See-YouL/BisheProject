/**
 * @file bsp_sound.h
 * @author Eric(jff517680@gmail.com)
 * @brief 蜂鸣器驱动
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __BSP_SOUND_H
#define __BSP_SOUND_H

#include "stm32f10x.h"

#define SOUND_GPIO_PORT GPIOA
#define SOUND_GPIO_CLK RCC_APB2Periph_GPIOA
#define SOUND_GPIO_PIN GPIO_Pin_5

typedef enum {
    Sound_Open = 0,
    Sound_Close = 1
}Sound_State;

void Sound_GPIO_Config(void);
void Sound_Controller(Sound_State state);

#endif // __BSP_SOUND_H