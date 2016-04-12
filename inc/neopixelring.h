/**
  *****************************************************************************
  * @file    neopixelring.h
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
#ifndef __NEOPIXELRING_H
#define __NEOPIXELRING_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>


typedef void (*NeopixelRingEffect)(uint8_t buffer[][3], bool reset);

#define BLACK_EFFECT            0
#define SPIN_EFFECT2            1
#define WHITE_SPIN_EFFECT       2
#define COLOR_SPIN_EFFECT       3
#define DOUBLE_SPIN_EFFECT      4
#define SOLID_COLOR_EFFECT      5
#define LED_TEST_EFFECT         6
#define SIREN_EFFECT            7
#define BOAT_EFFECT             8

#define RING_TIMER  50

extern uint16_t NeopixelRingTimer;
extern uint16_t NeopixelRingStart;

extern void NeopixelRingInit(void);
extern uint16_t NeopixelRingTest(void);
extern void NeopixelRingWorker(void);

#endif //__NEOPIXELRING_H__


/*****************************END OF FILE*************************************/
