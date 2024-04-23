/**
 * @file bsp_esp8266.h
 * @author Eric(jff517680@gmail.com)
 * @brief ESP8266声明
 * @version 0.1
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef  __BSP_ESP8266_H
#define	 __BSP_ESP8266_H



#include "stm32f10x.h"
// #include "common.h"
#include <stdio.h>
#include <stdbool.h>
#include "bsp_SysTick.h" // Delay
#include "bsp_gsm_gprs.h" // GSM_DELAY()



#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/********************************** 用户需要设置的参数**********************************/
#define      macUser_ESP8266_ApSsid                       "1"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "12345678"           //要连接的热点的密钥

#define      macUser_MQTT_Username                 "mqtt_test01&k13n6SLl76W"
#define      macUser_MQTT_Passwd                 "57d13701638add78834b88f320f4d45b77521b98daf8583dc64bc25a4c134b0e"
#define      macUser_MQTT_Passwd                 "57d13701638add78834b88f320f4d45b77521b98daf8583dc64bc25a4c134b0e"


/******************************* ESP8266 数据类型定义 ***************************/
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;



/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strEsp8266_Fram_Record;

extern struct STRUCT_USARTx_Fram strUSART_Fram_Record;

/******************************** ESP8266 连接引脚定义 ***********************************/
#define      macESP8266_CH_PD_APBxClock_FUN                   RCC_APB2PeriphClockCmd  /*重定义开启ESP8266 CH_PD 端口的时钟函数*/
#define      macESP8266_CH_PD_CLK                             RCC_APB2Periph_GPIOB   /* 定义ESP8266 CH_PD 引脚端口时钟 */
#define      macESP8266_CH_PD_PORT                            GPIOB                 /* 定义ESP8266 CH_PD 引脚端口*/
#define      macESP8266_CH_PD_PIN                             GPIO_Pin_8            /* 定义ESP8266 CH_PD 引脚*/

#define      macESP8266_RST_APBxClock_FUN                     RCC_APB2PeriphClockCmd/* 重定义开启ESP8266 RST 端口的时钟函数 */
#define      macESP8266_RST_CLK                               RCC_APB2Periph_GPIOB  /* 定义ESP8266 RST 引脚端口时钟 */
#define      macESP8266_RST_PORT                              GPIOB                 /* 定义ESP8266 RST 引脚端口*/
#define      macESP8266_RST_PIN                               GPIO_Pin_9            /* 定义ESP8266 RST 引脚*/

 

#define      macESP8266_USART_BAUD_RATE                       115200                /* 定义ESP8266的串口波特率为 115200*/      

#define      macESP8266_USARTx                                USART3                /* 定义ESP8266串口为串口3*/
#define      macESP8266_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd/* 重定义开启ESP8266 USART 的时钟函数 */
#define      macESP8266_USART_CLK                             RCC_APB1Periph_USART3 /* 重定义ESP8266 USART3 */
#define      macESP8266_USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd /* 重定义开启ESP8266 USART GPIO 的时钟函数 */
#define      macESP8266_USART_GPIO_CLK                        RCC_APB2Periph_GPIOB   /* 定义ESP8266 串口 PIO 时钟*/    
#define      macESP8266_USART_TX_PORT                         GPIOB                  /* 定义ESP8266 串口 发送 GPIO 端口*/
#define      macESP8266_USART_TX_PIN                          GPIO_Pin_10            /* 定义ESP8266 串口 发送 GPIO 引脚*/
#define      macESP8266_USART_RX_PORT                         GPIOB                  /* 定义ESP8266 串口 接收 GPIO 端口*/
#define      macESP8266_USART_RX_PIN                          GPIO_Pin_11            /* 定义ESP8266 串口 接收 GPIO 引脚*/
#define      macESP8266_USART_IRQ                             USART3_IRQn             /* 重定义ESP8266 串口 中断*/
#define      macESP8266_USART_INT_FUN                         USART3_IRQHandler      /* 重定义ESP8266 串口 中断函数*/



/*********************************************** ESP8266 函数宏定义 *******************************************/
#define     macESP8266_Usart( fmt, ... )           USART_printf ( macESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
// #define     macPC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )
//#define     macPC_Usart( fmt, ... )                

#define     macESP8266_CH_ENABLE()                 GPIO_SetBits(macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN)
#define     macESP8266_CH_DISABLE()                GPIO_ResetBits(macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN)

#define     macESP8266_RST_HIGH_LEVEL()            GPIO_SetBits(macESP8266_RST_PORT, macESP8266_RST_PIN)
#define     macESP8266_RST_LOW_LEVEL()             GPIO_ResetBits(macESP8266_RST_PORT, macESP8266_RST_PIN)



/****************************************** ESP8266 函数声明 ***********************************************/
void                     ESP8266_Init                        ( void );
void                     ESP8266_Rst                         ( void );
bool                     ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, u32 waittime );
bool                     ESP8266_AT_Test                     ( void );
bool                     ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
bool                     ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
bool                     ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool                     ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
bool                     ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool                     ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t                  ESP8266_Get_LinkStatus              ( void );
uint8_t                  ESP8266_Get_IdLinkStatus            ( void );
uint8_t                  ESP8266_Inquire_ApIp                ( char * pApIp, uint8_t ucArrayLength );
bool                     ESP8266_UnvarnishSend               ( void );
void                     ESP8266_ExitUnvarnishSend           ( void );
bool                     ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
char *                   ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );
bool                     ESP8266_DHCP_CUR                    ( void );
void ESP8266_Config_Test(void);



#endif
