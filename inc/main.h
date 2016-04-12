/**
  ******************************************************************************
  * @file    main.h
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  * "Copyright MSU Kalabhavan Electrical Engineering Department. This document
  * is supplied by MSU Kalabhavan Electrical Engineering Department on the
  * express understanding that it is to be treated as confidential and that
  * [subject to any rights, contractual or otherwise, which the Indian Government
  * or any Indian Governmental Department or any Third Party may have] it may not
  * be copied, used or disclosed to others in whole or in part for any purpose
  * except as authorised in writing by MSU Kalabhavan EE Department."
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_it.h"

#include "usart2.h"
#include "msg.h"
#include "push_button.h"
#include "leds.h"
#include "adc.h"
#include "utility.h"
#include "neopixelring.h"
#include "ws2812.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variable ------------------------------------------------------------*/
extern __IO uint32_t TimingDelay;

/* Exported functions ------------------------------------------------------- */
extern void Delay(__IO uint32_t nTime);
extern void TimingDelay_Decrement(void);

#endif /* __MAIN_H */


/*****************************END OF FILE*************************************/
