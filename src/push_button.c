/**
  *****************************************************************************
  * @file    push_button.c 
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   Push button on/off test
  *****************************************************************************
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
const char  *PbOnOffTest = "\x1b[2;10H User Push-Button On/Off Test";
const char  *PbStatus = "\x1b[10;10H User PB Status = ";

/* Private variables ---------------------------------------------------------*/
__IO uint32_t UserButtonPressed = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void WordToBit(uint16_t number);

/**
 * @brief  Push Button ON/OFF test
 * @param  None
 * @retval None
 */
uint32_t PBOnOffTest(void)
{
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO); 

  UserButtonPressed = 0; 

	/* Display Test type */
  usart2_printf("%s", PbOnOffTest);    /* "User Push-Button On/Off Test" */
  PrintEscape();

  while(1)
  {
    UserButtonPressed = STM_EVAL_PBGetState(BUTTON_USER);

    usart2_printf("%s", PbStatus);    /* "User Push-Button On/Off Test" */

    if(UserButtonPressed == 0)
    {
      usart2_printf("Off");
    }
    else
    {
      usart2_printf("On ");
    }

    /* Insert 50ms delay */
    Delay(50);

    if(usart2_rxdata_avail)
    {
      if(usart2_rxdata == In_ESC)        /* ESC ? */
      {
        usart2_rxdata_avail = 0;         /* Yes, exit */
        return(0);
      }
    }
  }
} 

/**
 * @brief  print out 16bit word into binary format
 * @param  number
 * @retval None
 */
void WordToBit(uint16_t number)
{
	uint8_t j;
  uint16_t num, numbit;
  uint16_t mask;

	num = number;
	mask = 0x0001;	/* 0000 0000 0000 0001 binary */

	for(j=0; j<16; j++)
  {
    numbit = (mask & num) ? 1 : 0;   /* bit 1 or 0 */
    usart2_printf("%d01 ", numbit);	 /* print bit */

    mask <<= 1;		                   /*  shift mask to left */
  }
}

/**
 * @brief  print out 16bit word into binary format
 * @param  number
 * @retval None
 */
void HexToBit(uint16_t number)
{
	uint16_t j, bit, num;
	uint16_t mask;

	num = number;
	mask = 0x8000;	                     /* 1000 0000 0000 0000 binary */

	usart2_printf("Binary of %x04 is: ", num);

	for(j=0; j<16; j++)
	{
    bit = (mask & num) ? 1 : 0;        /* bit 1 or 0 */
    usart2_printf("%d01 ", bit);       /* print bit */
    if(j==7)
    {                                  /* print dash between */
      usart2_printf("-- ");	           /* 	bytes	 */
    }
    else
	  {
      mask >>= 1;                      /* shift mask to right */
		}
  }
}


/*****************************END OF FILE*************************************/


