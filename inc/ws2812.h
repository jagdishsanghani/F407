/**
  *****************************************************************************
  * @file    ws2812.h
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   
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
#ifndef __WS2812_H
#define __WS2812_H

/* Includes -----------------------------------------------------------------*/
#include "main.h"


/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/

/* Definition for DMAx's NVIC */
#define ws2812DmaIsr                     DMA1_Stream5_IRQHandler 

/* Exported constants -------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/
extern uint32_t NeopixelTimeOut;
extern uint16_t allLedDone;
extern uint16_t ws2812_init;

/* Exported functions -------------------------------------------------------*/
extern void ws2812Init(void);
extern void ws2812Send(uint8_t (*color)[3], int len);
extern void ws2812DmaIsr(void);


#endif

/****************************** END OF FILE **********************************/

