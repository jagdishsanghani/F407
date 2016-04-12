/**
  *****************************************************************************
  * @file    leds.c
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   Leds flasher
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
const char  *Leds4Test = "\x1b[2;14H Four Leds On/Off Test";

/* Private variables ---------------------------------------------------------*/
uint16_t  LedsTest = 0;
uint16_t  LedsDelay;
static __IO uint32_t LedTimingDelay;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Leds On/Off test
 * @param  None
 * @retval None
 */
uint32_t LedsOnOffTest(void)
{
  LedsInit();
  LedsOff();
  usart2_printf("%s", Leds4Test);

  PrintEscape();
	
  while(1)
  {
    /* Toggle LD3 */
    STM_EVAL_LEDToggle(LED3);
    /* Insert 50 ms delay */
    Delay(50);
    /* Toggle LD5 */
    STM_EVAL_LEDToggle(LED5);
    /* Insert 50 ms delay */
    Delay(50);
    /* Toggle LD6 */
    STM_EVAL_LEDToggle(LED6);
    /* Insert 50 ms delay */
    Delay(50);
    /* Toggle LD4 */
    STM_EVAL_LEDToggle(LED4);
    /* Insert 50 ms delay */
    Delay(50);

    if(usart2_rxdata_avail)
    {
      if(usart2_rxdata == In_ESC)        /* ESC ? */
      {
        usart2_rxdata_avail = 0;         /* Yes, exit */
        LedsOff();
        return(0);
      }
    }
  }
} 

/**
 * @brief  Initialize Leds 
 * @param  None
 * @retval None
 */	
void LedsInit(void)
{
  /* Initialize LEDs and User Button available on STM32F3-Discovery board */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
}

/**
 * @brief  Leds OFF
 * @param  None
 * @retval None
 */	
void LedsOff(void)
{
  /* LEDs Off */
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED6);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDOff(LED5);
}

/**
 * @brief  Leds Toggle
 * @param  None
 * @retval None
 */	
void LedsToggle (void)
{
  /* Toggle All LEDs */
  STM_EVAL_LEDToggle(LED6);
  STM_EVAL_LEDToggle(LED3);
  STM_EVAL_LEDToggle(LED4);
  STM_EVAL_LEDToggle(LED5);
}

/**
 * @brief  Leds ON
 * @param  None
 * @retval None
 */	
void LedsOn(void)
{
  /* LEDs On */
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED6);
  STM_EVAL_LEDOn(LED4);
  STM_EVAL_LEDOn(LED5);
}

/**
  * @brief  Inserts a delay time for LED.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void LedDelay(__IO uint32_t nLedTime)
{ 
  LedTimingDelay = nLedTime;

  while(LedTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void LedTimingDelay_Decrement(void)
{
  if (LedTimingDelay != 0x00)
  { 
    LedTimingDelay--;
  }
}

/******************************** End of File *********************************/
