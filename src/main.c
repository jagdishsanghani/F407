/**
  *****************************************************************************
  * @file    main.c 
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   Main program body
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
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimeOut =1;
__IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);


/* ---------------------------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main (void)
{
  static 	uint8_t  state = 0;

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
	*/     

  USART2_Init();
  LedsInit();
 
  STM_EVAL_LEDOff(LED4); 

  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    while (1);     /* Capture error */ 
  }

  while(1)         /* main loop */
  {		
    clr_scr_rst_cur();
    print_main_menu();	

    state = get_atoi();
    switch(state)
    {
      case 1:
          clr_scr_rst_cur();
          LedsOnOffTest();
          break;

      case 2:
          clr_scr_rst_cur();
          PBOnOffTest();
          break;

      case 3:
          clr_scr_rst_cur();
          adc_temp_sensor_test();
          break;

      case 4:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 5:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 6:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 7:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 8:
          clr_scr_rst_cur();
          NeopixelRingTest();
          break;

      case 9:
          clr_scr_rst_cur();
          /* add Test program here */
          break;


      case 10:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 11:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 12:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 13:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 14:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      case 15:
          clr_scr_rst_cur();
          /* add Test program here */
          break;

      default:
          break;
    }
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}



/*****************************END OF FILE*************************************/

