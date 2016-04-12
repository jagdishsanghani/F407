/**
  ******************************************************************************
  * @file    leds.h
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   This file contains all the functions prototypes for the Leds.
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
#ifndef __LEDS_H
#define __LEDS_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/

extern uint16_t  LedsTest;

extern void LedDelay(__IO uint32_t nTime);
extern void LedTimingDelay_Decrement(void);

/* Exported functions ------------------------------------------------------- */
extern uint32_t LedsOnOffTest(void);
extern void LedsInit(void);
extern void LedsOff(void);
extern void LedsToggle (void);
extern void LedsOn(void);

#endif /* __LEDS_H */

/******************************** End of File *********************************/
