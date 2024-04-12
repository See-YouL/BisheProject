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

#define GSM_INIT_DEBUG_ENABLE    1
#define GSM_INIT_DEBUG_DISABLE 0
#define DIALUP_DEBUG_ENABLE      1
#define DIALUP_DEBUG_DISABLE     0
#define MESSEAGE_DEBUG_ENABLE    1
#define MESSEAGE_DEBUG_DISABLE   0
#define HR501_DEBUG_ENABLE    1
#define HR501_DEBUG_DISABLE   0
#define MQ2_DEBUG_ENABLE 1
#define MQ2_DEBUG_DISABLE 0

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
    Key_GPIO_Config();   // Key GPIO Init
    SysTick_Init();      // Systick Init
    LED_GPIO_Config();   // LED GPIO Init
    EXTI_SR501_Config(); // SR501 EXTI Init
    Sound_GPIO_Config(); // Sound GPIO Init
    MQ2_Config();        // MQ-2 EXTI Init

    /*-Test Begin-------------------------------------------------------------*/
    printf("Test Begin \n");

#if GSM_INIT_DEBUG_ENABLE
    /*-GSM Module Initialization----------------------------------------------*/
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

    /*-WHILE BEGIN------------------------------------------------------------*/
    while (1) {

#if HR501_DEBUG_DISABLE
        /*-Human detection----------------------------------------------------*/
        if (0 == GPIO_ReadInputDataBit(SR501_INT_GPIO_PORT, SR501_INT_GPIO_PIN)) {
            Sound_Controller(Sound_Close); // Close sound
            GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN); // Turn off LED
            printf("Clear the state of HR501 Warn \n");
        }
        GSM_DELAY(1000); // Delay 1s
#endif

#if MQ2_DEBUG_DISABLE
    /*-Smoke alarms-----------------------------------------------------------*/
    if(MQ2_Normal == MQ2_Read())
    {
        printf("Delete MQ2-Warn \n");
        Sound_Controller(Sound_Close); // Close Sound
        printf("Clear the state of MQ2 Warn \n");
    }
    GSM_DELAY(1000);
#endif

    }
}
