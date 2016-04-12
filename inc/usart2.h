/**
  *****************************************************************************
  * @file    usart2.h 
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   This file contains all the functions prototypes for the USART 
  *          firmware library.
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
#ifndef __USART2_H
#define __USART2_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

	 //  CIFER VDU control bytes  

#define    In_DELETE        0x7F      /* ASCII <DEL>  */
#define    In_ESC           0x1B      /* ASCII ESCAPE */
#define    In_CR            0x0D      /* ASCII Carriage Return */
#define    In_BACKSPACE     '\x8'      /* ASCII BACK_SPACE */

#define    Out_BACKSPACE    '\x8'     /* ASCII BACK_SPACE */
#define    Out_SPACE        '\x20'    /* ASCII BACK_SPACE */
#define    Out_DELETE       "\x8 \x8" /* BS, SPACE and BS. Erase character on the screen */
#define    EOL              '\r'      /* ASCII <CR> */
#define    Out_CR           0x0D      /* ASCII Carriage Return */
#define    LF               '\n'      /* ASCII Carriage Return */


/* Exported functions ------------------------------------------------------- */

extern void USART2_Init(void);
extern int8_t usart2_putchar (int8_t c);
extern int8_t usart2_getchar(void);
extern int usart2_printf(const char *format, ...);

/* Exported global variables -------------------------------------------------*/
extern uint8_t  esc_flag;

extern uint8_t usart2_rxdata;
extern uint8_t usart2_rxdata_avail;

extern uint8_t usart2_txdata;
extern uint8_t usart2_tx_restart;



#endif /* __USART1_H */

/*****************************END OF FILE*************************************/
