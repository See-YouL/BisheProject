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
#define ESP8266_DEBUG_ENABLE      1
#define ESP8266_DEBUG_DISABLE     0

/*-Global Variable -----------------------------------------------------------*/
// char *rebuff; // 拨号使用

/*-MAIN-----------------------------------------------------------------------*/
int main(void)
{
    /*-Peripherals Initialization---------------------------------------------*/
    SysTick_Init();      // Systick Init
    USART_Config();      // DEBUG USART1 Init
    GSM_USART_Config();  // GSM USART2 Init
    GSM_Init(); // GSM Init Test
    // Key_GPIO_Config();   // Key GPIO Init
    // LED_GPIO_Config();   // LED GPIO Init
    // Sound_GPIO_Config(); // Sound GPIO Init
    // MQ2_Config();        // MQ-2 EXTI Init
    // ESP8266_Init(); // ESP8266 Init
    OLED_Init();    // OLED Init
    EXTI_SR501_Config(); // SR501 EXTI Init

    /*-Test Begin-------------------------------------------------------------*/
    printf("Test Begin \n");




#if DIALUP_DEBUG_DISABLE
    /*-Dial-up----------------------------------------------------------------*/
    char *rebuff; // 拨号使用
    printf("Dial-up test after 5 seconds...\n");
    GSM_DELAY(5000);                     // Delay
    gsm_call("13704161756");             // Call the number
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
    char MessageNum[] = "13704161756"; // 短信接收号码
    printf("SMS starts after 5 seconds... \n");
    GSM_DELAY(5000); // Delay 5s

    if (gsm_sms((char *)MessageNum, "Test 04-26") == GSM_TRUE) { // Send Message
        printf(":Message is already send to  %s \n", MessageNum);
    } else {
        printf("If the SMS message cannot be sent, Please confirm that the destination MessageNumber is valid \n");
    }
    GSM_DELAY(2000); // Delay 2s
#endif

/*-WHILE BEGIN----------------------------------------------------------------*/
while (1) {

#if OLED_DEBUG_ENABLE
/*-OLED-----------------------------------------------------------------------*/

        /* OLED Always on Display */
        OLED_Clear();
        OLED_ShowString(8, 14, "SECURITY SYSTEM", 12);
        OLED_ShowString(0, 36, "Your state are safe", 12);
        OLED_Refresh();
        GSM_DELAY(10000);
#endif

#if HR501_DEBUG_ENABLE
    /*-Human detection----------------------------------------------------*/
    if (1 == GPIO_ReadInputDataBit(SR501_INT_GPIO_PORT, SR501_INT_GPIO_PIN)) {
        printf("HR501 Warn \n");

        /* OLED显示报警信息 */
        OLED_Clear();
        OLED_ShowString(8, 16, "!!!WARNNING!!!", 16);
        OLED_ShowString(20, 32, "Intrusion Alarm", 12);
        OLED_Refresh();

        GSM_Dialup();
        GSM_Messeage();

        Sound_Controller(Sound_Close);               // Close sound

    }
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

#if ESP8266_DEBUG_DISABLE 
    /*-ESP8266----------------------------------------------------------------*/
    if(0 == ESP8266_AT_Test())
    {
        printf("111\n");
    }
#endif

    }
}
