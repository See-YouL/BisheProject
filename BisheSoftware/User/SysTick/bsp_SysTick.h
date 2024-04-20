/**
 * @file bsp_SysTick.h
 * @author Eirc(jff517680@gmail.com)
 * @brief ????????
 * @version 0.1
 * @date 2024-04-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
void Delay_ms(__IO u32 nTime);
//#define Delay_ms(x) Delay_us(100*x)	 //��λms

#endif /* __SYSTICK_H */
