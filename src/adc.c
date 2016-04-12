/**
  ******************************************************************************
  * @file    adc.c 
  * @author  FTE Quadcopter Application Team
  * @version V1.0
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const char  *Adc_TempSensorTest = "\x1b[2;14H ADC Temperature sensor Test";

const char  *FormulaForTempSensor = "\x1b[4;10H Temperature(in deg C)={(Vsense-V25)/Avg_Slope}+25";

const char  *RawTempSensorValue = "\x1b[6;10H Raw Temp Sensor Value = ";
const char  *RawVrefintValue = "\x1b[8;10H Raw Vrefint Value = ";


const char  *Adc_Temp = "\x1b[12;10H Temperature in degC = ";

const char * adc_menu[6] = {"\r\n ADC Temperature Sensor Test \n\r\n",
      " 1. Temp sensor polling mode test\n\r",
      " 2. Temp sensor interrupt mode test\n\r",
      " 3. Temp sensor interrupt DMA mode test\n\r\n",
      "Select option (1-3) and press ENTER",
      "\x1b[24;10H Press ESCAPE to exit\x1b[8;37H",
  };

/* Private macro -------------------------------------------------------------*/
#define NUM_OF_SAMPLES                4
#define ADC_TEMPERATURE_V25         760  /* mv */
#define ADC_TEMP_AVG_SLOPE           25  /* mV/C */
#define ADC_TEMP_AVG_SLOPE_DIVIDER   10  /* mV/C */
#define ADC_RESOLUTION             4096  /* 12 bit */

/* Private variables ---------------------------------------------------------*/
uint16_t  AdcTempSenTest = 0;

uint16_t  ADC_Value = 0;
uint32_t  TempSensor = 0;
uint32_t  Vrefint = 0;

uint16_t  ADC_SampleCount = 0;
uint16_t  ADC_ValueRdy = 0;
uint16_t  ADC_rawValue = 0;

uint16_t TemperatureC;
uint16_t PrecisionTempInDegC;

uint16_t AdcDelay;

/***** IT ********************/
uint16_t IT_AdcTempSenTest = 0;
uint16_t IT_AdcDelay;

/* Private function prototypes -----------------------------------------------*/
void ADC_Config(void);
int32_t adc_value_to_temp (uint16_t value, uint16_t steps_per_volt);
uint16_t adc_read(ADC_TypeDef* ADCx, uint8_t channel, uint8_t ADC_SampleTime);
uint8_t TempSensorTest (void);

void PrintTempTest(void);
void PrintTempFormula(void);
void print_adc_menu(void);

/**
  * @brief  ADC1 Channel TempSensorVrefint configuration
  * @note   This function Configure the ADC peripheral  
            1) Enable peripheral clocks
            2) Configure ADC1 Channel16 (TempSensor) 
  * @param  None
  * @retval None
  */
//void AdcTempSensorConfig(void)
void ADC_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
    
  /* Enable peripheral clocks *************************************************/
  RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);


   /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

 /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel16 (TempSensor) configuration ******************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);

  /* Enable TempSensor channel */
  ADC_TempSensorVrefintCmd(ENABLE); 

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC1, ENABLE);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
uint16_t adc_read(ADC_TypeDef* ADCx, uint8_t channel, uint8_t ADC_SampleTime)
{
  /* Configure channel */
  ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime);
 
  /* check if conversion was started, if not start */
  ADC_SoftwareStartConv(ADCx);
 
  /* wait for end of conversion */
  while((ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET));
 
  return ADC_GetConversionValue(ADCx);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
int32_t adc_value_to_temp (uint16_t value, uint16_t steps_per_volt)
{
  int32_t mv;

  /* convert reading to millivolts */
  mv = ((uint32_t) value * 1000) / steps_per_volt ;

  return (((mv - ADC_TEMPERATURE_V25)* ADC_TEMP_AVG_SLOPE_DIVIDER / ADC_TEMP_AVG_SLOPE)  + 25);
}

/**
 * @brief  int16_t adc_steps_per_volt(uint16_t vref_value)
 * @param  None
 * @retval None
 */
int16_t adc_steps_per_volt(uint16_t vref_value)
{
  return ((vref_value * 100)/ 121);  /* assume 1.21V internal voltage */
}

/*************#### Interrupt Mode ####*********************/

/**
 * @brief  
 * @param  None
 * @retval None
 */
void ADC_Config_IT(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  NVIC_InitTypeDef      NVIC_InitStructure;

  /* Enable peripheral clocks *************************************************/
  RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);


   /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

 /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* Enable Vref & Temperature channel */
  ADC_TempSensorVrefintCmd(ENABLE);
 
  /* Configure channels */
  /* Temp sensor */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);

  /* VREF_int (2nd) */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 2, ADC_SampleTime_480Cycles);

//  ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);
 
  /* Enable ADC interrupts */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

  /* Configure and enable ADC1 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC1, ENABLE);

}


uint16_t IT_adc_read(ADC_TypeDef* ADCx, uint8_t ChannelNbr, uint8_t ADC_SampleTime)
{
  /* Configure channel */
  ADC_RegularChannelConfig(ADCx, ChannelNbr, 1, ADC_SampleTime);
 
  /* check if conversion was started, if not start */
  ADC_SoftwareStartConv(ADCx);
 
  /* wait for end of conversion */
  while(!ADC_ValueRdy){/* wait */};

  ADC_ValueRdy = 0;
   
  return (ADC_rawValue);
}


/**
  * @brief  The ADC notifies the application for each end of regular conversion (EOC) event.
  * The ADC sets the EOC flag as soon as a new regular conversion data is available
  * in the ADC_DR register. An interrupt can be generated if bit EOCIE is set. 
  * EOC flag is cleared by the software either by writing 1 to it or by reading ADC_DR.
  * @param  None
  * @retval None
  */
void ADC_IRQHandler (void)
{
  ADC_rawValue = ADC_GetConversionValue(ADC1);    /* Read ADC Data and clear EOC flag */

  /* acknowledge interrupt */
  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
 
  ADC_ValueRdy = 1; /* New Data is ready */
}

/*****************#####End of Interrupt Mode #####********************/

/******************######...TEST...############***********************/

/**
 * @brief  
 * @param  None
 * @retval None
 */
void PrintTempTest(void)
{
  usart2_printf(Adc_TempSensorTest);  /* "\x1b[24;10H Press ESCAPE to exit"; */
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void PrintTempFormula(void)
{
  usart2_printf(FormulaForTempSensor);  /* "\x1b[24;10H Press ESCAPE to exit"; */
}


/**
  * @brief  uint8_t TempSensorTest (void)
  * @param  None
  * @retval None
  */
uint8_t TempSensorTest (void)
{
  uint32_t RawTempValue;
  uint16_t TempInDegC;

  uint32_t VrefintValue;
  uint16_t AdcStepPerVolt;

	uint16_t i;

  AdcTempSenTest = 1;
  AdcDelay = 100;

  PrintTempTest();
  PrintTempFormula();
  PrintEscape();

  ADC_Config();

  while(1)
	{
    /* takes average 4 readings for TempSensor */ 
		for(i = 0; i < NUM_OF_SAMPLES; i++)
    {
      TempSensor += adc_read(ADC1, ADC_Channel_TempSensor, ADC_SampleTime_480Cycles);
    }

    /* takes average 4 readings for Vrefint */ 
		for(i = 0; i < NUM_OF_SAMPLES; i++)
    {
      Vrefint += adc_read(ADC1, ADC_Channel_Vrefint, ADC_SampleTime_480Cycles);
    }

    RawTempValue = TempSensor / NUM_OF_SAMPLES;
    TempSensor = 0;     /* Reset to zero */

    VrefintValue = Vrefint / NUM_OF_SAMPLES;
    Vrefint = 0;        /* Reset to zero */

    AdcStepPerVolt = adc_steps_per_volt((uint16_t)VrefintValue);

    TempInDegC = adc_value_to_temp ((uint16_t) RawTempValue, AdcStepPerVolt);

    usart2_printf("%s", RawTempSensorValue);       /* Print "ADC value in bits =" */
    usart2_printf("%d 5",RawTempValue);

    usart2_printf("%s", RawVrefintValue);      /* Print "ADC Voltage =" */
    usart2_printf("%d 5",VrefintValue);

    usart2_printf("%s", Adc_Temp);           /* Print "Temperature degC = " */
    usart2_printf("%d 5",TempInDegC);

    if(usart2_rxdata_avail)
    {
      if(usart2_rxdata == In_ESC)             /* ESC ? */
      {
        usart2_rxdata_avail = 0;              /* Yes, exit */
        AdcTempSenTest = 0;
        STM_EVAL_LEDOff(LED5);
        AdcDelay = 0;
        return(0);
      }
    }
  }/*end of while loop */
}

/************************####INTERRUPT MODE ######*************************/

/**
  * @brief  uint8_t TempSensorInterruptModeTest (void)
  * @param  None
  * @retval None
  */

uint8_t TempSensorITModeTest (void)
{
  uint32_t IT_RawTempValue;
  uint16_t IT_TempInDegC;

  uint32_t IT_VrefintValue;
  uint16_t IT_AdcStepPerVolt;

	uint16_t i;

  IT_AdcTempSenTest = 1;
  IT_AdcDelay = 100;

  PrintTempTest();
  PrintTempFormula();
  PrintEscape();

  ADC_Config_IT();

  while(1)
	{
    /* takes average 4 readings for TempSensor */ 
		for(i = 0; i < NUM_OF_SAMPLES; i++)
    {
      TempSensor += IT_adc_read(ADC1, ADC_Channel_TempSensor, ADC_SampleTime_480Cycles);
    }

    /* takes average 4 readings for Vrefint */ 
		for(i = 0; i < NUM_OF_SAMPLES; i++)
    {
      Vrefint += IT_adc_read(ADC1, ADC_Channel_Vrefint, ADC_SampleTime_480Cycles);
    }

    IT_RawTempValue = TempSensor / NUM_OF_SAMPLES;
    TempSensor = 0;     /* Reset to zero */

    IT_VrefintValue = Vrefint / NUM_OF_SAMPLES;
    Vrefint = 0;        /* Reset to zero */

    IT_AdcStepPerVolt = adc_steps_per_volt((uint16_t)IT_VrefintValue);

    IT_TempInDegC = adc_value_to_temp ((uint16_t) IT_RawTempValue, IT_AdcStepPerVolt);

    usart2_printf("%s", RawTempSensorValue);       /* Print "ADC value in bits =" */
    usart2_printf("%d 5",IT_RawTempValue);

    usart2_printf("%s", RawVrefintValue);      /* Print "ADC Voltage =" */
    usart2_printf("%d 5",IT_VrefintValue);

    usart2_printf("%s", Adc_Temp);           /* Print "Temperature degC = " */
    usart2_printf("%d 5",IT_TempInDegC);

    if(usart2_rxdata_avail)
    {
      if(usart2_rxdata == In_ESC)             /* ESC ? */
      {
        usart2_rxdata_avail = 0;              /* Yes, exit */
        AdcTempSenTest = 0;
        STM_EVAL_LEDOff(LED6);
        AdcDelay = 0;
        return(0);
      }
    }
  }/*end of while loop */
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int adc_temp_sensor_test (void)
{
  static 	uint8_t  state;
 
  while(1)
  {		
    clr_scr_rst_cur();
    print_adc_menu();

    state = get_atoi();

    switch(state)
    {
      case 1:
          clr_scr_rst_cur();
          TempSensorTest();
          break;

      case 2:
          clr_scr_rst_cur();
          TempSensorITModeTest();
          break;

      case 3:
          clr_scr_rst_cur();
//        TempSensorDMATest();         
          break;

      default:
          return(0);
    }
  }
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void print_adc_menu(void)
{
  uint8_t count;

  for( count = 0; count<6; ++count)
  {
    usart2_printf(adc_menu[count]);
  }
}

/******************************** End of File *********************************/
