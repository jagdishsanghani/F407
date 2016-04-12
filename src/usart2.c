/**
  *****************************************************************************
  * @file    usart2.c 
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   communication module
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
#include <stdarg.h>
#include <stdio.h>
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t  esc_flag = 0;

uint8_t usart2_rxdata = 0;
uint8_t usart2_rxdata_avail = 0;

uint8_t usart2_txdata = 0;
uint8_t usart2_tx_restart = 1;

/* Private function prototypes -----------------------------------------------*/
static char low_level_get(void);

/* Private functions ---------------------------------------------------------*/
void USART2_Init(void);
int8_t usart2_putchar (int8_t c);
int8_t usart2_getchar(void);
int usart2_printf(const char *format, ...);


/**
 * @brief  This function initializes USART2 module
 * @param  None
 * @retval None
 */

void USART2_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* Enable USART clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Connect USART2 pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  /* USART2_TX */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); /* USART2_RX */ 

    /* USART configuration */
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_Init(USART2, &USART_InitStructure);

    /* Enable the USART2 Receive interrupt: this interrupt is generated when */
    /* the USART2 receive data register is not empty */

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable USART */
    USART_Cmd(USART2, ENABLE);
}

/**
  * @brief  Transmits single data through the USARTx peripheral.
  * @param  USARTx: Select the USART peripheral. This parameter can be one of the 
  *         following values: USART1 or USART2 or USART2 or UART4 or UART5.
  * @param  Data: the data to transmit.
  * @retval None
  */

int8_t usart2_putchar (int8_t c)
{
  do{} while(!usart2_tx_restart);
  /* Transmit Data */
  USART2->DR = (c & (uint16_t)0x01FF);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {/* do nothing */ };
		
  return (c);
}

/**
  * @brief  
  * @param  
  *         
  * @retval 
  */

static char low_level_get(void)
{
	do { } while (!usart2_rxdata_avail);
	usart2_rxdata_avail = 0;              /* clear RxData0_Avail flag */
	esc_flag = 0;                         /* clear EscapeFlag */
	return (usart2_rxdata);               /* return data */
}

/**
  * @brief  
  * @param  
  *         
  * @retval 
  */

int8_t usart2_getchar(void)
{
  char key;
		
	for(;;)
	{
		key = low_level_get();
		if ( key == 0x0D )
		{
			return(key);
		}
		else if ( key == In_DELETE || key == In_BACKSPACE )
	 	{
			return(key);
		}
		else if ( key == In_ESC )
		{
			esc_flag = 1;
			return ( In_ESC );
		}
		else if ( key >= ' ' )
		{
			return (key);
		}
		else
		{
			usart2_putchar ( '\7' );
		}
	}
}

/**
  * @brief  This function handles USART2 interrupt Handler.
  * @param  
  *         
  * @retval 
  */
void USART2_IRQHandler (void)
{
	/* if Receive interrupt */
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    USART2->SR &= ~USART_FLAG_RXNE;	 /* clear rx interrupt */
		usart2_rxdata = (uint8_t)(USART2->DR & (uint16_t)0x01FF); /* read char from usart1 */
    usart2_rxdata_avail = 1;           /* Flag that Rx Data available */
  }
  /* if Transmit interrupt */	
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {
    USART2->SR &= ~USART_FLAG_TXE;	   /* clear tx interrupt */
    usart2_tx_restart = 1;             /* ready to transmit another byte */
  }
}

/*
*******************************************************************************
 * Function: int printf(const char *format, ...)
 * Arg1:
 * Arg2:
 *
 * Return(s):  return with valid character or nothing 
 * Description: Description:  Supports standard formats %c %s %d %x
 *   %d and %x requires non-standard qualifiers,%dfn, %xfn:-
 *        f supplies a fill character
 *        n supplies a field width
 *   %u	prints the 16 bit unsigned integer in hex format
 *
 *******************************************************************************
 */
int usart2_printf(const char *format, ...)
{
  static const uint16_t width_dec[5] = {1, 10, 100, 1000, 10000};
  static const uint16_t width_hex[4] = {0x1, 0x10, 0x100, 0x1000};
  static const int8_t hex[]= "0123456789ABCDEF";

  uint32_t u_val, div_val, base;
  int8_t format_flag, fill_char;

  int8_t 	*ptr;
  va_list   ap;
  va_start(ap, format);

  for(;;)
  {    /* until full format string read */
    while((format_flag = *format++) != '%')
    {
      if(!format_flag)
      {                                /* until '%' or '\0' */
        return (0);
      }
      usart2_putchar(format_flag);
    }

    switch(format_flag = *format++)
    {
      case 'c':
          format_flag = va_arg(ap, int);

          default:
            usart2_putchar(format_flag);

          continue;

      case 's':
          ptr = va_arg(ap, int8_t * );
          format_flag = *ptr++;
          while(format_flag)
          {
            usart2_putchar(format_flag);
            format_flag = *ptr++;
          }
          continue;

      case 'u':
          base = 16;
          div_val = 0x1000;
          u_val = va_arg(ap, unsigned int);
          do
          {
            usart2_putchar(hex[u_val / div_val]);
            u_val %= div_val;
            div_val /= base;
          }while(div_val);

          continue;

      case 'd':
          base = 10;
          fill_char = *format++;
          format_flag = ( *format++ ) - '1';
          div_val = width_dec[format_flag];
          u_val = va_arg(ap,int);

          if(((int)u_val) < 0)
          {   /* applied to unsigned type, result still unsigned */
            u_val = - u_val; 
            usart2_putchar('-');
          }

          goto  CONVERSION_LOOP;

      case 'x':
          base = 16;
          fill_char = *format++;
          format_flag = (*format++) - '1';
          div_val = width_hex[format_flag];
          u_val = va_arg(ap, unsigned int);

          CONVERSION_LOOP:
          while(div_val > 1 && div_val > u_val)
          {
            div_val /= base;
            usart2_putchar(fill_char);
          }

          do
          {
            usart2_putchar(hex[u_val / div_val]);
            u_val %= div_val;
            div_val /= base;
          }while(div_val);

    }/* end of switch statement */
  }
}


/*****************************END OF FILE*************************************/
