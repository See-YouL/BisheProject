/**
 * @file main.c
 * @author Eric(jff517680@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-04-08
 *
 * @copyright Copyright (c) 2024
 *
 */

/*-INCLUDES------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "bsp_usart.h"
#include "bsp_gsm_usart.h"
#include "bsp_SysTick.h"
#include "bsp_gsm_gprs.h"
#include "bsp_key.h"
#include "bsp_sdfs_app.h"
#include <string.h>
#include "bsp_led.h"
#include "bsp_sr501.h"
#include "bsp_sound.h"
#include "bsp_mq.h"

const char num[]     = "15694266242"; // LSY Phone Number
char namenum[20 * 4] = {0}, str[512] = {0}, gbkstr[256] = {0}, namegbk[256] = {0};
char testbuff[100];
uint8_t len;
char *rebuff;

/*-MAIN-----------------------------------------------------------------------*/
int main(void)
{
/*-Variable Initialization----------------------------------------------------*/
    static uint8_t timecount = 0;
    // char num[20]             = {0};
    uint8_t newmessadd = 0, IsRead = 0;
    uint8_t testCard = 0;

/*-Peripherals Initialization-------------------------------------------------*/
    USART_Config(); // DEBUG USART1 Init
    GSM_USART_Config(); // GSM USART2 Init
    Key_GPIO_Config(); // Key GPIO Init
    SysTick_Init(); // Systick Init
    LED_GPIO_Config(); // LED GPIO Init
    EXTI_SR501_Config(); // SR501 EXTI Init
    Sound_GPIO_Config(); // Sound GPIO Init
    MQ2_Config(); // MQ-2 EXTI Init

/*--------------------Test Begin----------------------------------------------*/
    printf("Test Begin \n");

/*--------------------GSM module Initialization-------------------------------*/
#if 1
    // Check GSM respond
    while (gsm_init() != GSM_TRUE) {
        printf("ERROR: GSM module not initialized \n")
        printf("The GSM module response test is not working properly \n");
        printf("Please check whether the module is connected or whether the power switch is turned on \n");
        GSM_DELAY(500);
    }
    printf("Passed the GSM module response test, and the Phone-Call test started after 5 seconds \n");
    GSM_DELAY(5000);

    // Set the text mode
    if (gsm_cmd("AT+CMGF=1\r", "OK", 100) != GSM_TRUE) {
        printf("ERROR: Text mode setting \n")
    }
    GSM_DELAY(1000);

    printf("Waiting for GSM module initializationa \n");
    while (IsInsertCard() != GSM_TRUE) {
        if (++testCard > 20) {
            printf("ERROR: Phone card not detcted \n");
            printf("If the phone card is not detected, please power off and reconnect the phone card \n");
        }
        GSM_DELAY(1000);
    }
#endif

#if 0
    /* 鎷ㄥ彿娴嬭瘯 */
    printf("鍒濆�嬪寲瀹屾垚锛�5绉掑悗寮€濮嬫嫧鍙锋祴璇�...  \n");
    /* 寤舵椂浜旂�� */
    GSM_DELAY(5000);
    /* 鎷ㄦ墦鐢佃瘽 */
    gsm_call("15694266242"); // 鎷ㄦ墦lsy鐢佃瘽娴嬭瘯
    rebuff = gsm_waitask(0);
    /* 鍝嶅簲OK, 琛ㄧずGSM妯″潡姝ｅ父鎺ユ敹鍒板懡浠� */
    if (strstr(rebuff, "ATD") != NULL) 
    {
        printf("姝ｅ湪鍛煎彨 \n");
        /* 娓呴櫎鎺ユ敹缂撳啿鍖� */
        GSM_CLEAN_RX();          
        /* 閲嶆柊绛夊緟娑堟伅 */
        rebuff = gsm_waitask(0); 
        /* 鍝嶅簲NO CARRIER, 閫氳瘽缁撴潫 */
        if (strstr(rebuff, "NO CARRIER") != NULL)
        {
            printf("閫氳瘽缁撴潫 \n");
        } 
        /* 鍝嶅簲NO ANSWER, 鏃犱汉鎺ュ惉 */
        else if (strstr(rebuff, "NO ANSWER") != NULL)
        {
            printf("鎮ㄦ嫧鎵撶殑鐢佃瘽鏆傛椂鏃犱汉鎺ュ惉, 璇风◢鍚庡啀鎷� \n");
        }
    }
    /* 娓呴櫎鎺ユ敹缂撳啿鍖� */
    GSM_CLEAN_RX();
#endif

#if 1
    /* 鐭�淇℃祴璇� */
    printf("鍒濆�嬪寲瀹屾垚锛�5绉掑悗寮€濮嬪彂閫佺煭淇℃祴璇�...  \n");
    /* 寤舵椂5绉掑啀鍙戦€佸懡浠ゅ埌妯″潡 */
    GSM_DELAY(5000);

    // 鍙戦€佽嫳鏂囩煭淇�

    if (gsm_sms((char *)num, "Test 02") == GSM_TRUE) {
        printf(": %s \n", num);
    } else {
        printf("鐭�淇″彂閫佸け璐ワ紝璇风‘璁ょ洰鏍囧彿鐮佹湁鏁� \n");
    }
    GSM_DELAY(2000);

    // 涓�鑻辨枃鐭�淇★紝瀹為檯娴嬭瘯鏃惰�锋妸鐢佃瘽鍙风爜淇�鏀规垚瑕佹帴鏀剁煭淇＄殑鎵嬫満鍙�
    if (gsm_sms((char *)num, "短信测试 01") == GSM_TRUE) {
        printf("已发送至: %s \n", num);
    } else {
        printf("鐭�淇″彂閫佸け璐ワ紝璇风‘璁ょ洰鏍囧彿鐮佹湁鏁� \n");
        while (1)
            ;
    }

#endif

    while (1) {
#if 0
        /**
         * 鎺ユ敹鐢佃瘽 
         */
        if (timecount >= 100)
        {
            /* 鏈夋潵鐢电數璇� */
            if (IsRing(num) == GSM_TRUE)
            {
                printf("Ringing: From:%s \n", num);
                printf("璇锋寜涓婯EY2鎺ュ惉鎴栬€呮寜涓婯EY1鎸傛柇 \n");
            }
            /* 閲嶇疆璁℃暟鍥犲瓙 */
            timecount = 0;
        }
        /* 鎸変笅KEY2鎺ュ惉 */
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON) 
        {
            GSM_HANGON();
        }
        /* 鎸変笅KEY1鎸傛柇 */
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON) 
        {
            GSM_HANGOFF();
        }
        /* 璁℃椂鍥犲瓙閫掑�� */
        timecount++;
        /* 鐭�寤舵椂 */
        GSM_DELAY(10);
#endif

#if 0
        /**
         * 鎺ユ敹鐭�淇�
         */
        /* 璇ュ欢鏃跺彲浠ラ€氳繃娴嬭瘯鍚庤繘琛屽垹闄�*/
        GSM_DELAY(1000);
        newmessadd = IsReceiveMS();
        if (newmessadd)
        {
            IsRead = readmessage(newmessadd, namenum, str);

            //			printf("newmessadd=%d,IsRead:%d \n",newmessadd,IsRead);
            if (IsRead)
            {
                printf("鏂扮煭淇� \n");

                hexuni2gbk(namenum, namegbk);
                hexuni2gbk(str, gbkstr);
                printf("鍙戜欢浜�: %s \n", namegbk);
                printf("鍐呭��: \n");
                printf("%s \n", gbkstr);
            }
        }
#endif

#if 0
        /**
         * 浜轰綋妫€娴�
         * 鍦ㄤ富鍑芥暟涓�娓呴櫎LED(鐔勭伃LED)
         * 鍦‥XTI涓�鏂�涓�鐐逛寒LED
         */
        if(0 == GPIO_ReadInputDataBit(SR501_INT_GPIO_PORT,SR501_INT_GPIO_PIN))
		{			
            Sound_Controller(Sound_Close);
            /* 娓呴櫎LED */
			GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		}

        GSM_DELAY(1000);
#endif

#if 0
    /**
     * 鏈夊�虫皵浣撴�€娴� 
     * 涓诲嚱鏁颁腑鍏抽棴铚傞福鍣�
     * 涓�鏂�涓�鎵撳紑铚傞福鍣�
     */
    if(MQ2_Normal == MQ2_Read())
    {
        // DEBUG 娓呴櫎鐑熼浘鎶ヨ��
        printf("Delete MQ2-Warn \n");
        // 鍏抽棴铚傞福鍣�
        Sound_Controller(Sound_Close);
    }
    GSM_DELAY(1000);

#endif
    }
}
