/**
  ******************************************************************************
  * @file    adc.h 
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    01-December-2015
  * @brief   ADC1 Ambient temperature measurement
  ******************************************************************************
  * @attention
  * "Copyright MSU Kalabhavan Electrical Engineering Department. This document
  * is supplied by MSU Kalabhavan Electrical Engineering Department on the
  * express understanding that it is to be treated as confidential and that
  * [subject to any rights, contractual or otherwise, which the Indian Government
  * or any Indian Governmental Department or any Third Party may have] it may not
  * be copied, used or disclosed to others in whole or in part for any purpose
  * except as authorised in writing by MSU Kalabhavan EE Department."
  *****************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint16_t AdcTempSenTest;
extern uint16_t AdcDelay;

extern uint16_t IT_AdcTempSenTest;
extern uint16_t IT_AdcDelay;

/* Exported macro ------------------------------------------------------------*/
  #define ADCx_CLK                 RCC_APB2Periph_ADC1
//  #define ADCx_DR_ADDRESS          ((uint32_t)0x4001204C)
  
/* Exported functions ------------------------------------------------------- */
extern void ADC_Config(void);
extern uint8_t TempSensorTest (void);
extern uint8_t TempSensorITModeTest (void);
extern int adc_temp_sensor_test (void);

#endif /* __ADC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
