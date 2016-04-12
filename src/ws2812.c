/**
  ******************************************************************************
  * @file    ws2812.c
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   The minimum is to have 2 leds (1 per half buffer) in the buffer, this
  *          consume 42Bytes and will trigger the DMA interrupt at ~2KHz.
  *          Putting 2 there will divide by 2 the interrupt frequency but will also 
  *          double the memory consumption.
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
#include "main.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef        TIM_OCInitStructure;
static GPIO_InitTypeDef         GPIO_InitStructure;
static DMA_InitTypeDef          DMA_InitStructure;
static NVIC_InitTypeDef         NVIC_InitStructure;

uint16_t allLedDone = NULL;
uint16_t ws2812_init = NULL;

/* Private define ------------------------------------------------------------*/
#define LED_PER_HALF 1

#define TIMING_ONE   75        /* high value */
#define TIMING_ZERO  29        /* low value */

#define TIMING_PERIOD  105-1   /* 84 MHz/0.8 MHz=105,-1 due to start from zero */

/* Private macro -------------------------------------------------------------*/
#define bzero(b, len)  (memset((b), '\0', (len)), (void) 0)

/* Private variables ---------------------------------------------------------*/
static int current_led = 0;
static int total_led = 0;
static uint8_t (*color_led)[3] = NULL;
uint32_t NeopixelTimeOut;

/* Private structures -----------------------------------------------*/
#pragma anon_unions
static union {
    uint16_t buffer[2*LED_PER_HALF*24];
    struct {
        uint16_t begin[LED_PER_HALF*24];
        uint16_t end[LED_PER_HALF*24];
    };
} led_dma;

/* Private function prototypes -----------------------------------------------*/

/******************************************************************************
  * @brief WS2812 Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral
  *           - TIM3_CH2 , DMA1_Channel_5/Stream5
  * @param  None
  * @retval None
  * --------------------------------------------------------------------------- 
  *    TIM3 input clock (TIM3CLK) is set to APB1 clock (PCLK2), since APB1 
  *    prescaler is 0.   
  *    TIM3CLK = PCLK  
  *    PCLK = HCLK
  *    => TIM1CLK = HCLK = SystemCoreClock
  *  
  *  TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
  *  SystemCoreClock is set to 168 MHz for STM32F4xx devices.
  *
  *  The objective is to configure TIM3_Channel-2 to generate PWM signal with a 
  *  frequency equal to 800 KHz:
  *    - TIM3_Period = ( (SystemCoreClock/105) - 1 ) = 1.25uSec
  *  and a variable duty cycle that is changed by the DMA after a specific number of
  *  Update DMA request.
  *  
  *  Note: 
  *   SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
  *   Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
  *   variable value. Otherwise, any configuration based on this variable will be incorrect.
  *   This variable is updated in three ways:
  *    1) by calling CMSIS function SystemCoreClockUpdate()
  *    2) by calling HAL API function HAL_RCC_GetSysClockFreq()
  *    3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency     
  *  -----------------------------------------------------------------------------
  *  Initialize TIM3 peripheral as follows:
  *    + Period = TimerPeriod (To have an output frequency equal to 800 KHz)
  *    + Repetition Counter = 0
  *    + Prescaler = 0
  *    + ClockDivision = 0
  *    + Counter direction = Up
  *****************************************************************************/

void ws2812Init(void)
{
	uint16_t PrescalerValue;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* GPIOB Configuration: TIM3 Channel 2 as alternate function push-pull */
  // Configure the GPIO PB5 TIM3_CH2 PWM output for WS2812
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure the GPIO PB4 for the LEDs
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //Map timer to alternate functions, TimeBased PWM output
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
	
	/* Compute the prescaler value */
	PrescalerValue = 0; 
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIMING_PERIOD;     // 800kHz -> Period of 1.25 uSec
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; //CK_PSC=84MHz, CK_CNT=84MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

//  TIM_Cmd(TIM3, ENABLE);                      // Go!!!
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); // PreLoad Timer-3, Channel-2 
	TIM_CtrlPWMOutputs(TIM3, ENABLE);           // enable Timer 3 ?????????????

	/* configure DMA */
	/* DMA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* DMA1 Channel2 Config */
	DMA_DeInit(DMA1_Stream5);

  // DMA Channel Config
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM3->CCR2;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)led_dma.buffer;    // this is the buffer memory
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  // ???????
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //????
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
  DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMA1_Stream5, DMA_IT_HT, ENABLE);

	/* TIM3 CC2 DMA Request enable */
	TIM_DMACmd(TIM3, TIM_DMA_CC2, ENABLE);

  allLedDone = true;
  ws2812_init = true;
}

/**
  * @brief 
  * @param 
  * @retval None
  */
static void fillLed(uint16_t *buffer, uint8_t *color)
{
  int i;

  for(i=0; i<8; i++) // GREEN data
	{
    buffer[i] = ((color[1]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
	}
	for(i=0; i<8; i++) // RED
	{
	  buffer[8+i] = ((color[0]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
	}
	for(i=0; i<8; i++) // BLUE
	{
	  buffer[16+i] = ((color[2]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
	}
}

/**
  * @brief 
  * @param 
  * @retval None
  */

void ws2812Send(uint8_t (*color)[3], int len)
{
  int i;

	if(len<1) return;
	
	//Wait for previous transfer to be finished
  while(!allLedDone) {/* Wait for MAX_DELAY */ };

	// Set interrupt context ...
	current_led = 0;
	total_led = len;
	color_led = color;
	
  for(i=0; (i<LED_PER_HALF) && (current_led<total_led+2); i++, current_led++)
  {
    if (current_led<total_led)
    {
      fillLed(led_dma.begin+(24*i), color_led[current_led]);
    }
    else
    {
      bzero(led_dma.begin+(24*i), sizeof(led_dma.begin));
    }
  }
    
  for(i=0; (i<LED_PER_HALF) && (current_led<total_led+2); i++, current_led++)
  {
    if (current_led<total_led)
    {
      fillLed(led_dma.end+(24*i), color_led[current_led]);
    }
    else
    {
      bzero(led_dma.end+(24*i), sizeof(led_dma.end));
    }
  }

	 // load number of bytes to be transferred	
	DMA1_Stream5->NDTR = sizeof(led_dma.buffer) / sizeof(led_dma.buffer[0]);
	DMA_Cmd(DMA1_Stream5, ENABLE); 			// enable DMA channel 2
	TIM_Cmd(TIM3, ENABLE);              // Go!!!
}

/**
  * @brief 
  * @param 
  * @retval None
  */
void ws2812DmaIsr(void)
{
    uint16_t * buffer;
    int i;

    STM_EVAL_LEDOn(LED5);    

    if (total_led == 0)
    {
      TIM_Cmd(TIM3, DISABLE);
    	DMA_Cmd(DMA1_Stream5, DISABLE);
    }

    /* #define DMA_IT_HTIF5   ((uint32_t)0x20004400) */    
    if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_HTIF5))
    {
      DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_HTIF5);
      buffer = led_dma.begin;
    }
    
    if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
    {
      DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
      buffer = led_dma.end;
    }

    for(i=0; (i<LED_PER_HALF) && (current_led<total_led+2); i++, current_led++)
    {
      if (current_led<total_led)
      {
        fillLed(buffer+(24*i), color_led[current_led]);
      }
      else
      {
        bzero(buffer+(24*i), sizeof(led_dma.end));
      }
    }
    
    if (current_led >= total_led+2)
    {
	    TIM_Cmd(TIM3, DISABLE); 					// disable Timer 3
	    DMA_Cmd(DMA1_Stream5, DISABLE); 	// disable DMA stream4
	    
	    total_led = 0;
      allLedDone = true;
      STM_EVAL_LEDOff(LED5);
    }
}


/******************************** End of File *********************************/
