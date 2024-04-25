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

/**
 * @brief PIN
 *
 * PA1 -> Sound_GND
 * PA2 -> SIM800A_SRX
 * PA3 -> SIM800A_STX
 * PA4 -> OLED_CS
 * PA5 -> OLED_SCK
 * PA7 -> OLED_SDA
 * PA8 -> SR501_OUT
 *
 * PB0 -> OLED_RES
 * PB1 -> OLED_DC
 * PB3 -> MQ2_DO // 烟雾
 * PB4 -> MQ4_DO // 天然气
 * PB5 -> MQ5_DO // 液化气
 * PB6 -> MQ9_DO // 可燃气体
 * PB8 -> ESP8266_EN
 * PB9 -> ESP8266_RST
 * PB10 -> ESP8266_RX
 * PB11 -> ESP8266_TX
 *
 */

/*-INCLUDES------------------------------------------------------------------*/
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "bsp_usart.h"     // DEBUG
#include "bsp_gsm_usart.h" // GSM
#include "bsp_SysTick.h"   // Delay
#include "bsp_gsm_gprs.h"  // GSM
// #include "bsp_key.h" // Key
#include "bsp_sdfs_app.h" // GSM
// #include "bsp_led.h" // Led
#include "bsp_sr501.h" // Sr501
#include "bsp_sound.h" // Sound
// #include "bsp_mq.h" // Mq2// 需要换口PA4占用
#include "bsp_oled.h"    // Oled
#include "bsp_esp8266.h" // ESP8266

#define GSM_INIT_DEBUG_ENABLE  1
#define GSM_INIT_DEBUG_DISABLE 0
#define DIALUP_DEBUG_ENABLE    1
#define DIALUP_DEBUG_DISABLE   0
#define MESSEAGE_DEBUG_ENABLE  1
#define MESSEAGE_DEBUG_DISABLE 0
#define HR501_DEBUG_ENABLE     1
#define HR501_DEBUG_DISABLE    0
#define MQ2_DEBUG_ENABLE       1
#define MQ2_DEBUG_DISABLE      0
#define OLED_DEBUG_ENABLE      1
#define OLED_DEBUG_DISABLE     0

/*-Global Variable -----------------------------------------------------------*/
const char num[]     = "15694266242"; // LSY Phone Number
char namenum[20 * 4] = {0}, str[512] = {0},
                  gbkstr[256] = {0}, namegbk[256] = {0};
char testbuff[100];
uint8_t len;
char *rebuff;

/*-MAIN-----------------------------------------------------------------------*/
int main(void)
{
    /*-Variable Initialization------------------------------------------------*/
    static uint8_t timecount = 0;
    // char num[20]             = {0}; // 修改num变量避免与全局变量冲突
    uint8_t newmessadd = 0, IsRead = 0;
    uint8_t testCard = 0;

    /*-Peripherals Initialization---------------------------------------------*/
    USART_Config();      // DEBUG USART1 Init
    GSM_USART_Config();  // GSM USART2 Init
    // Key_GPIO_Config();   // Key GPIO Init
    SysTick_Init();      // Systick Init
    // LED_GPIO_Config();   // LED GPIO Init
    EXTI_SR501_Config(); // SR501 EXTI Init
    Sound_GPIO_Config(); // Sound GPIO Init
    // MQ2_Config();        // MQ-2 EXTI Init
    ESP8266_Init(); // ESP8266 Init
    OLED_Init();    // OLED Init

    /*-Test Begin-------------------------------------------------------------*/
    printf("Test Begin \n");

#if 1

#endif

#if GSM_INIT_DEBUG_DISABLE
    /*-GSM Module Initialization----------------------------------------------*/
    // Check GSM respond
    while (gsm_init() != GSM_TRUE) {
        printf("ERROR: GSM module not initialized \n");
        printf("The GSM module response test is not working properly \n");
        printf("Please check whether the module is connected or whether the power switch is turned on \n");
        GSM_DELAY(500);
    }
    printf("Passed the GSM module response test, and the Phone-Call test started after 5 seconds \n");
    GSM_DELAY(5000);

    // Set the text mode
    if (gsm_cmd("AT+CMGF=1\r", "OK", 100) != GSM_TRUE) {
        printf("ERROR: Text mode setting \n");
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

#if DIALUP_DEBUG_DISABLE
    /*-Dial-up----------------------------------------------------------------*/
    printf("Passed the module response test, and started the dial-up test after 5 seconds...\n");
    GSM_DELAY(5000);                     // Delay 5s
    gsm_call("15694266242");             // Call the number
    rebuff = gsm_waitask(0);             // Receive a response
    if (strstr(rebuff, "ATD") != NULL) { // The response is OK
        printf("Calling now \n");
        GSM_CLEAN_RX();          // Clear the receive buffer
        rebuff = gsm_waitask(0); // Wait for it to disappear again

        if (strstr(rebuff, "NO CARRIER") != NULL) { // Respond NO CARRIER, End Call
            printf("End Call \n");
        } else if (strstr(rebuff, "NO ANSWER") != NULL) { // Pespond NO ANSWER, End Call
            printf("No Answer \n");
        }
    }
    GSM_CLEAN_RX(); // Clear the receive buffer
#endif

#if MESSEAGE_DEBUG_DISABLE
    /*-Message----------------------------------------------------------------*/
    printf("Passed the module response test, and started the dial-up test after 5 seconds...\n");
    printf("The initialization is complete, and the SMS test starts after 5 seconds...");
    GSM_DELAY(5000); // Delay 5s

    if (gsm_sms((char *)num, "Test 02") == GSM_TRUE) { // Send Message
        printf(":Message is already send to  %s \n", num);
    } else {
        printf("If the SMS message cannot be sent, Please confirm that the destination number is valid \n");
    }
    GSM_DELAY(2000); // Delay 2s
#endif

/*-WHILE BEGIN----------------------------------------------------------------*/
while (1) {
#if 1 
    /*-ESP8266----------------------------------------------------------------*/
    if(0 == ESP8266_AT_Test())
    {
        printf("111\n");
    }
#endif

#if HR501_DEBUG_DISABLE
        /*-Human detection----------------------------------------------------*/
        if (0 == GPIO_ReadInputDataBit(SR501_INT_GPIO_PORT, SR501_INT_GPIO_PIN)) {
            Sound_Controller(Sound_Close);               // Close sound
            GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN); // Turn off LED
            printf("Clear the state of HR501 Warn \n");
        }
        GSM_DELAY(1000); // Delay 1s
#endif

#if MQ2_DEBUG_DISABLE
        /*-Smoke alarms-----------------------------------------------------------*/
        if (MQ2_Normal == MQ2_Read()) {
            printf("Delete MQ2-Warn \n");
            Sound_Controller(Sound_Close); // Close Sound
            printf("Clear the state of MQ2 Warn \n");
        }
        GSM_DELAY(1000);
#endif

#if OLED_DEBUG_DISABLE
        /*-OLED---------------------------------------------------------------*/
        // u8 t;
        // OLED_ColorTurn(0);   // 0 normal display, 1 inverted color display
        // OLED_DisplayTurn(0); // 0 normal display 1 screen flip display
        // OLED_Refresh();      // OLED refresh
        // t = ' ';
        // GSM_DELAY(500);
        // OLED_Clear();
        // u32 i = 123;
        // OLED_ShowChar(8, 16, a, 12);
        // GSM_DELAY(500);
        // OLED_ShowChar(0, 0, a, 16);
        // OLED_ShowNum(0, 0, i, 5, 16);
        // GSM_DELAY(500);
        // OLED_ShowChar(8, 16, a, 24);
        // GSM_DELAY(500);
        // OLED_ShowString(8, 16, "ZHONGJINGYUAN", 16);
        // OLED_ShowString(20, 32, "2014/05/01", 16);
        // OLED_ShowString(0, 48, "ASCII:", 16);
        // OLED_ShowString(63, 48, "CODE:", 16);
        // OLED_ShowChar(48, 48, t, 16); // 显示ASCII字符
        // t++;
        // if (t > '~') t = ' ';
        // OLED_ShowNum(103, 48, t, 3, 16);
        // OLED_Refresh();
        // GSM_DELAY(500);
        // OLED_Clear();
        // OLED_ShowChinese(0, 0, 0, 16);   // 16*16 中
        // OLED_ShowChinese(16, 0, 0, 24);  // 24*24 中
        // OLED_ShowChinese(24, 20, 0, 32); // 32*32 中
        // OLED_ShowChinese(64, 0, 0, 64);  // 64*64 中
        // OLED_Refresh();
        // GSM_DELAY(500);
        // OLED_Clear();
        // OLED_ShowString(0, 0, "ABC", 12);  // 6*12 “ABC”
        // OLED_ShowString(0, 12, "ABC", 16); // 8*16 “ABC”
        // OLED_ShowString(0, 28, "ABC", 24); // 12*24 “ABC”
        // OLED_Refresh();
        // GSM_DELAY(500);
        // OLED_ScrollDisplay(11, 4);
#endif

    }
}
