/**
 ******************************************************************************
 * @file    main.c
 * @author  Eric
 * @version V1.0
 * @date    2022-April-05
 * @brief   主程序
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./gsm_gprs/bsp_gsm_usart.h"
#include "./systick/bsp_SysTick.h"
#include "./gsm_gprs/bsp_gsm_gprs.h"
#include "./key/bsp_key.h"
#include "bsp_sdfs_app.h"
#include <string.h>

/* 修改接收短信的手机号码 */
const char num[]     = "15694266242"; // LSY
char namenum[20 * 4] = {0}, str[512] = {0}, gbkstr[256] = {0}, namegbk[256] = {0};
char testbuff[100];
uint8_t len;
char *rebuff;

int main(void)
{
    static uint8_t timecount = 0;
    char num[20]             = {0};
    uint8_t newmessadd = 0, IsRead = 0;
    uint8_t testCard = 0;

    /* DEBUG USART1 初始化 */
    USART_Config();
    /* GSM USART2 初始化 */
    GSM_USART_Config();
    /* 按键初始化 */
    Key_GPIO_Config();
    /* 系统定时器初始化 */
    SysTick_Init();

    /* 程序测试 */
    printf("程序测试开始 \n");

    /* 测试模块响应是否正常 */
    while (gsm_init() != GSM_TRUE) {
        printf("GSM模块响应测试不正常 \n");
        printf("请检查GSM模块的链接或电源开关 \n");
        GSM_DELAY(500);
    }
    printf("通过了GSM模块响应测试, 5s后开始进行拨号测试 \n");
    GSM_DELAY(5000); 

	//先执行次设置文本模式
	if(gsm_cmd("AT+CMGF=1\r","OK", 100) != GSM_TRUE)
	{
		printf("设置文本模式错误 \n");
	}
	GSM_DELAY(1000); 	

    printf("正在等待GSM模块初始化... \n");
	while(IsInsertCard() != GSM_TRUE)
	{
		
		if(++testCard >20)
		{
			printf("检测不到电话卡，请断电并重新接入电话卡 \n");
		}
		GSM_DELAY(1000); 		
	}		
    /* 拨号测试 */
    printf("初始化完成，5秒后开始拨号测试...  \n");
    /* 延时五秒 */
    GSM_DELAY(5000);
    /* 拨打电话 */
    gsm_call("15694266242"); // 拨打lsy电话测试
    rebuff = gsm_waitask(0);
    /* 响应OK, 表示GSM模块正常接收到命令 */
    if (strstr(rebuff, "ATD") != NULL) 
    {
        printf("正在呼叫 \n");
        /* 清除接收缓冲区 */
        GSM_CLEAN_RX();          
        /* 重新等待消息 */
        rebuff = gsm_waitask(0); 
        /* 响应NO CARRIER, 通话结束 */
        if (strstr(rebuff, "NO CARRIER") != NULL)
        {
            printf("通话结束 \n");
        } 
        /* 响应NO ANSWER, 无人接听 */
        else if (strstr(rebuff, "NO ANSWER") != NULL)
        {
            printf("您拨打的电话暂时无人接听, 请稍后再拨 \n");
        }
    }
    /* 清除接收缓冲区 */
    GSM_CLEAN_RX();

    /* 短信测试 */
    printf("初始化完成，5秒后开始发送短信测试...  \n");
    /* 延时5秒再发送命令到模块 */
    GSM_DELAY(5000);    
    /* 发送英文短信 */
	if(gsm_sms((char *)num,"GSM Test") == GSM_TRUE)
    {
		printf("英文短信已发送至：%s，为方便测试，请在程序中修改接收短信的手机号码 \n",num);
    }
	else
	{
		printf("短信发送失败，请确认目标号码有效 \n");
	}
    GSM_DELAY(2000);    
	/* 中英文短信，实际测试时请把电话号码修改成要接收短信的手机号 */
	if(gsm_sms((char *)num,"GSM模块短信测试") == GSM_TRUE)
    {
		printf("中英文短信已发送至：%s，为方便测试，请在程序中修改接收短信的手机号码 \n",num);
    }
	else
	{
		printf("短信发送失败，请确认目标号码有效 \n");
		while(1);
	}		

    while (1) {
        if (timecount >= 100)
        {
            /* 有来电电话 */
            if (IsRing(num) == GSM_TRUE)
            {
                printf("Ringing: From:%s \n", num);
                printf("请按下KEY2接听或者按下KEY1挂断 \n");
            }
            /* 重置计数因子 */
            timecount = 0;
        }
        /* 按下KEY2接听 */
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON) 
        {
            GSM_HANGON();
        }
        /* 按下KEY1挂断 */
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON) 
        {
            GSM_HANGOFF();
        }
        /* 计时因子递增 */
        timecount++;
        /* 短延时 */
        GSM_DELAY(10);
        
        /* 等待接收短信 */
        /**
         * 该延时可以通过测试进行删除 
         * 将拨号和短信置于同一延时下
         */
        GSM_DELAY(1000);
        newmessadd = IsReceiveMS();
        if (newmessadd)
        {
            IsRead = readmessage(newmessadd, namenum, str);

            //			printf("newmessadd=%d,IsRead:%d \n",newmessadd,IsRead);
            if (IsRead)
            {
                printf("新短信 \n");

                hexuni2gbk(namenum, namegbk);
                hexuni2gbk(str, gbkstr);
                printf("发件人: %s \n", namegbk);
                printf("内容: \n");
                printf("%s \n", gbkstr);
            }
        }
    }
}
