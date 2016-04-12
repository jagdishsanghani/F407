/**
  ******************************************************************************
  * @file    utility.c 
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
  *****************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************
 *  Function:     int16_t get_atoi(void)
 *                
 *  Description:  The int16_t get_atoi(void) converts a argument string into an int value.
 *	              The string is expected to be of the form:
 *                [whitespace][sign][digits]
 *  
 *  Parameters:   none
 * 
 *  Returns:      The atoi function returns the integer value as an
 *                int variable. The return value is undefined if it
 *                is to large to fit an int variable. 
 *****************************************************************************/

int16_t get_atoi(void)
{
  unsigned char StringBuffer[8];
  uint16_t pointer=0;/* Pointer in buffer */
  uint16_t count=0;/* character count */
  uint16_t digit = 6;

  int result = 0;
  char cp;
  // int sign;

  for(;;)
  {
    for(;;)
    {
      cp = usart2_getchar();      /* get input character */

      if ( cp == In_ESC )         /* if ESCAPE pressed then exit */
      {
        return ( cp );
      }

      else if ( cp == In_CR )     /* CARRIAGE RETURN ? */
      {
        if(count == 0)            /* any characters at all ? */
        {
          continue;               /* no, so get another character */
        }

        break;
      }
                  /* delete or back space */
      else if ( cp == In_DELETE || cp == In_BACKSPACE )
      {
        if(pointer==0)            /* any characters entered */
        {
          continue;               /* no, so get another character */
        }

        /* erase character on the screen */
        usart2_putchar( Out_BACKSPACE );
        usart2_putchar( Out_SPACE );
        usart2_putchar( Out_BACKSPACE );

        pointer--;                /* decrement pointer */
        count--;                  /* decrement character count */
        continue;
      }

      else if (cp>= '0' && cp <='9')   /* is character between 0 to 9? */
      {
        if(pointer < digit)
        {
          StringBuffer[pointer] = cp;  /* save character and */
          pointer++;                   /* increment pointer */
          count++;                     /* increment count */
          usart2_putchar(cp);          /* echo character */

          continue;                    /*continue;and get some more */
        }
      }
      else
      {
        usart2_putchar(cp);            /* echo character */

         /* erase character on the screen */
        usart2_putchar( Out_BACKSPACE );
        usart2_putchar( Out_SPACE );
        usart2_putchar( Out_BACKSPACE );

        usart2_putchar('\7');          /* ring the bell */
        continue;
      }
    }

		/* now convert the ascii string into 16 bit integer */
    pointer =0;                        /* point to first character entered */
    cp = StringBuffer[pointer];        /* read character */

    while(count != 0)
    {
      result *= 10;                    /* multiply buy 10 */
      result += cp-'0';                /* data - 30 */
      cp = StringBuffer[++pointer];    /* increment pointer */
      count--;                         /* decrement count */
    }
    return(result);
  }
}	

