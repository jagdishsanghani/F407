/** 
  ******************************************************************************
  * @file    stm32f4_discovery.h
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    14-August-2015
  * @brief   This file contains definitions for STM32F4-Discovery Kit's Leds and 
  *          push-button hardware resources.
  ******************************************************************************
 */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_H
#define __STM32F4_DISCOVERY_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum 
{
  LED4 = 0,
  LED3 = 1,
  LED5 = 2,
  LED6 = 3
} Led_TypeDef;

typedef enum 
{  
  BUTTON_USER = 0,
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;     

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             4

#define LED4_PIN                         GPIO_Pin_12
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK                    RCC_AHB1Periph_GPIOD  
  
#define LED3_PIN                         GPIO_Pin_13
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK                    RCC_AHB1Periph_GPIOD  
  
#define LED5_PIN                         GPIO_Pin_14
#define LED5_GPIO_PORT                   GPIOD
#define LED5_GPIO_CLK                    RCC_AHB1Periph_GPIOD  
  
#define LED6_PIN                         GPIO_Pin_15
#define LED6_GPIO_PORT                   GPIOD
#define LED6_GPIO_CLK                    RCC_AHB1Periph_GPIOD
  
/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          1  

/**
 * @brief Wakeup push-button
 */
#define USER_BUTTON_PIN                GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT          GPIOA
#define USER_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define USER_BUTTON_EXTI_LINE          EXTI_Line0
#define USER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
#define USER_BUTTON_EXTI_IRQn          EXTI0_IRQn 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
 
#endif /* __STM32F4_DISCOVERY_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

