/**
  *****************************************************************************
  * @file    msg.c 
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   display main menu on hyper-terminal
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
/* Private constants ---------------------------------------------------------*/
const char * PressEscape= "\x1b[24;10H Press ESCAPE to exit";
const char * ProgUndDev= "\x1b[10;10H Program under development";

const char *CrLf = "\r\n";            /* VT100- code for carriage return & line feed */
const char *ResetCursor = "\x1b[H";   /* VT100- code for set cursor to Home */
const char *ClearScreen = "\x1b[2J";  /* VT100- code for to clear entire screen */

const char * main_menu[19] = {"\t\tSTM32F407 DISCOVERY BOARD TEST  \n\r",
      "\tDiagnostic Software Ver 1.0  01-Jan-2016  J S\n\n\r",
      "\tMain Menu\n\r",
      " 1. LEDs ON/OFF Test\n\r",
      " 2. PushButton ON/OFF Test\n\r",
      " 3. CPU Temperature Sensor Test\n\r",
      " 4. Mems accelerometer Test\n\r",
      " 5. Audio Codec Test\n\r",
      " 6. Display PB, LEDs & Temp Sensor on LabView\n\r",
      " 7. Display Audio codec values on LabView\n\r",
      " 8. NeoPixelLeds ring Test\n\r",
      " 9. MPU9250 Roll,Pitch & Heading Test\n\r",
      "10. MS5611 Pressure sensor Test\n\r",
      "11. Madwick QUATERNION algorithm Test\n\r",
      "12. IIR filter Test\n\r",
      "13. Motors Test\n\r",
      "14. SensorFusion + PID Test\n\r",
      "15. GPS Test\n\r\n",

	"\tSelect option (1-15) and press ENTER  "
  };

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void print_main_menu(void);
void print_adc_menu(void);
void reset_cursor(void);
void clear_screen(void);
void clr_scr_rst_cur(void);
void cr_ret_ln_fd(void);


/**
 * @brief  
 * @param  None
 * @retval None
 */
void print_main_menu(void)
{
  uint8_t count;

  clr_scr_rst_cur();

  for( count = 0; count<19; ++count)
  {
    usart2_printf(main_menu[count]);
  }
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void reset_cursor(void)
{
  usart2_printf(ResetCursor);   /* escape sequence for vt220 ^[H sets cursor to Home */
					               /* same thing can be written as printf("%s","\033[2H"); */
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void clear_screen(void)
{
  usart2_printf(ClearScreen);   /* escape sequence for vt220 ESC[2J clears screen */
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void clr_scr_rst_cur(void)
{
  clear_screen();
  reset_cursor();
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void cr_ret_ln_fd(void)
{
  usart2_putchar('\r');	/* if not finished then do some more */
  usart2_putchar('\n');
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void PrintEscape(void)
{
  usart2_printf(PressEscape);  /* "\x1b[24;10H Press ESCAPE to exit"; */
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void PrintProgUnderDev(void)
{
  usart2_printf(ProgUndDev);  /* "\x1b[24;10H Press ESCAPE to exit"; */
}


/*****************************END OF FILE*************************************/

