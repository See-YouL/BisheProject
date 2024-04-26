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
//  * @brief  ����Ƕ�������жϿ�����NVIC
//  * @param  ��
//  * @retval ��
//  */
// static void NVIC_Configuration(void)
// {
//     NVIC_InitTypeDef NVIC_InitStructure;

//     /* ����NVICΪ���ȼ���2 */
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//     /* �����ж�Դ������1 */
//     NVIC_InitStructure.NVIC_IRQChannel = SR501_INT_EXTI_IRQ;
//     /* ������ռ���ȼ� */
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//     /* ���������ȼ� */
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//     /* ʹ���ж�ͨ�� */
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
// }

/**
 * @brief  ���� IOΪ�����
 * @param  ��
 * @retval ��
 */
void EXTI_SR501_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����GPIO�ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd(SR501_INT_GPIO_CLK, ENABLE);

    /*--------------------------GPIO����-----------------------------*/
    /* ѡ���õ���GPIO */
    GPIO_InitStructure.GPIO_Pin = SR501_INT_GPIO_PIN;
    /* ����Ϊ�������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(SR501_INT_GPIO_PORT, &GPIO_InitStructure);

}
/*********************************************END OF FILE**********************/
