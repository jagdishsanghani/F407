/**
  *****************************************************************************
  * @file    neopixelring.c
  * @author  FTE Quadcopter Application Team
  * @version V1.0
  * @date    01-December-2015
  * @brief   NeoPixel Ring 12 Leds effects/driver
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

#include <stdint.h>
#include <math.h>
#include "main.h"


/* Private define ------------------------------------------------------------*/

const char * NeopixelRingMenu[15] = {"\r\n Neopixel LEDs ring Test \r\n",
      " 1. LEDs All OFF test\n\r",
      " 2. LEDs White Spin effect test\n\r",
      " 3. LEDs Solid colour effect test\n\r",
      " 4. LEDs Boat effect test\n\r",
      " 5. LEDs Color Spin effect test\n\r",
      " 6. LEDs Spin effect2 test\n\r",
      " 7. LEDs Double Spin effect test\n\r",
      " 8. LEDs Brightness test\n\r",
      " 9. LEDs Siren effect test\n\r",
      "10. Stop NeopixelRing test\n\r\n",
      "Select option (1-10) and press ENTER\n\r\n",
      "By pressing escape, program is still running in background.\n\r",
      "To stop the program completely, run no.10 and then press escape.", 
     "\x1b[24;10H Press ESCAPE to exit\x1b[14;38H",
  };

/* Private function prototypes -----------------------------------------------*/
void print_NeopixelRingMenu(void);
void NeopixelRingDeInit(void);

static 	uint8_t worker_state = 1;
static uint16_t user_effect;
static uint16_t new_effect;

uint16_t NeopixelRingTimer = 0;
uint16_t NeopixelRingStart = 0;

/* xxxxxxxxxx ----------------------------------------------------------------*/
/*
 * To add a new effect just add it as a static function with the prototype
 * void effect(uint8_t buffer[][3], bool reset)
 *
 * Then add it to the effectsFct[] list bellow. It will automatically be
 * activated using the ring.effect parameter.
 *
 * The ring color needs to be written in the buffer argument. The buffer is not
 * modified in memory as long as reset is not 'true', see the spin effects for
 * and example.
 *
 * The log subsystem can be used to get the value of any log variable of the
 * system. See tiltEffect for an example.
 */

/**************** Some useful macros ***************/
#define NBR_LEDS  12

#define RED {0x10, 0x00, 0x00}
#define GREEN {0x00, 0x10, 0x00}
#define BLUE {0x00, 0x00, 0x10}
#define WHITE {0xff, 0xff, 0xff}
#define BLACK {0x00, 0x00, 0x00}

#define MAX(a,b) ((a>b)?a:b)
#define MIN(a,b) ((a<b)?a:b)

#define COPY_COLOR(dest, orig) dest[0]=orig[0]; dest[1]=orig[1]; dest[2]=orig[2]
#define ADD_COLOR(dest, o1, o2) dest[0]=(o1[0]>>1)+(o2[0]>>1);dest[1]=(o1[1]>>1)+(o2[1]>>1);dest[2]=(o1[2]>>1)+(o2[2]>>1);

#define LIMIT(a) ((a>255)?255:(a<0)?0:a)
#define SIGN(a) ((a>=0)?1:-1)
#define DEADBAND(a, b) ((a<b) ? 0:a)

#define LINSCALE(domain_low, domain_high, codomain_low, codomain_high, value) ((codomain_high - codomain_low) / (domain_high - domain_low)) * (value - domain_low) + codomain_low

#define SET_WHITE(dest, intensity) dest[0] = intensity; dest[1] = intensity; dest[2] = intensity;

/* xxxxxxxxxx ---------------------------------------------------------------------------*/
static uint32_t effect = 8;
static uint32_t neffect;

static uint8_t black[][3] = {BLACK, BLACK, BLACK,
                             BLACK, BLACK, BLACK,
                             BLACK, BLACK, BLACK,
                             BLACK, BLACK, BLACK,
                            };

/**************** Black (LEDs OFF) ***************/
static void blackEffect(uint8_t buffer[][3], bool reset)
{
  int i;

  if (reset)
  {
    for (i=0; i<NBR_LEDS; i++) {
      buffer[i][0] = 0;
      buffer[i][1] = 0;
      buffer[i][2] = 0;
    }
  }
}

/**************** White spin ***************/
static const uint8_t whiteRing[][3] = {{32, 32, 32}, {8,8,8}, {2,2,2},
                                       BLACK, BLACK, BLACK,
                                       BLACK, BLACK, BLACK,
                                       BLACK, BLACK, BLACK,
                                      };

static const uint8_t blueRing[][3] = {{64, 64, 255}, {32,32,64}, {8,8,16},
                                       BLACK, BLACK, BLACK,
                                       BLACK, BLACK, BLACK,
                                       BLACK, BLACK, BLACK,
                                      };
/*
static const uint8_t greenRing[][3] = {{64, 255, 64}, {32,64,32}, {8,16,8},
                                       BLACK, BLACK, BLACK,
                                       BLACK, BLACK, BLACK,
                                       BLACK, BLACK, BLACK,
                                      };

static const uint8_t redRing[][3] = {{64, 0, 0}, {16,0,0}, {8,0,0},
                                       {4,0,0}, {2,0,0}, {1,0,0},
                                       BLACK, BLACK, BLACK,
                                       BLACK, BLACK, BLACK,
                                      };

*/
/*******  whiteSpinEffect  ***********/
static void whiteSpinEffect(uint8_t buffer[][3], bool reset)
{
  int i;
  uint8_t temp[3];

  if (reset)
  {
    for (i=0; i<NBR_LEDS; i++) {
      COPY_COLOR(buffer[i], whiteRing[i]);
    }
  }

  COPY_COLOR(temp, buffer[0]);
  for (i=0; i<(NBR_LEDS-1); i++) {
    COPY_COLOR(buffer[i], buffer[i+1]);
  }
  COPY_COLOR(buffer[(NBR_LEDS-1)], temp);
}

static uint8_t solidRed=20, solidGreen=20, solidBlue=20;
// static float glowstep = 0.05;

/*******  solidColorEffect  ***********/
static void solidColorEffect(uint8_t buffer[][3], bool reset)
{
  int i;
  static float brightness=0;

  if (reset) brightness = 0;

  if (brightness<1) brightness += 0.05;
  else brightness = 1;

  for (i=0; i<NBR_LEDS; i++)
  {
    buffer[i][0] = solidRed*brightness;
    buffer[i][1] = solidGreen*brightness;
    buffer[i][2] = solidBlue*brightness;
  }
}

static const uint8_t green[] = {0x00, 0xFF, 0x00};
static const uint8_t red[] = {0xFF, 0x00, 0x00};
static const uint8_t blue[] = {0x00, 0x00, 0xFF};
static const uint8_t white[] = WHITE;
static const uint8_t part_black[] = BLACK;

/**********  boatEffect  *************/
static void boatEffect(uint8_t buffer[][3], bool reset)
{
  int i;

  uint8_t reds[] = {1,2,3,4,5};
  uint8_t greens[] = {7,8,9,10,11};
  uint8_t whites[] = {0};
  uint8_t blacks[] = {6};


  for (i=0; i<sizeof(reds); i++)
  {
    COPY_COLOR(buffer[reds[i]], red);
  }

  for (i=0; i<sizeof(greens); i++)
  {
    COPY_COLOR(buffer[greens[i]], green);
  }

  for (i=0; i<sizeof(whites); i++)
  {
    COPY_COLOR(buffer[whites[i]], white);
  }

  for (i=0; i<sizeof(blacks); i++)
  {
    COPY_COLOR(buffer[blacks[i]], part_black);
  }
}

/*****************************************************************************/
static const uint8_t colorRing[][3] = {{0,0,32}, {0,0,16}, {0,0,8},
                                       {0,0,4}, {16,16,16}, {8,8,8},
                                       {4,4,4},{32,0,0},{16,0,0},
                                       {8,0,0}, {4,0,0}, {2,0,0},
                                      };

/**************** colorSpinEffect ***************/
static void colorSpinEffect(uint8_t buffer[][3], bool reset)
{
  int i;
  uint8_t temp[3];

  if (reset)
  {
    for (i=0; i<NBR_LEDS; i++) {
      COPY_COLOR(buffer[i], colorRing[i]);
    }
  }

  COPY_COLOR(temp, buffer[0]);
  for (i=0; i<(NBR_LEDS-1); i++) {
    COPY_COLOR(buffer[i], buffer[i+1]);
  }
  COPY_COLOR(buffer[(NBR_LEDS-1)], temp);
}


/**************** spinEffect2 ***************/
static void spinEffect2(uint8_t buffer[][3], bool reset)
{
  int i;
  uint8_t temp[3];

  if (reset)
  {
    for (i=0; i<NBR_LEDS; i++) {
      COPY_COLOR(buffer[(NBR_LEDS-i)%NBR_LEDS], blueRing[i]);
    }
  }

  COPY_COLOR(temp, buffer[(NBR_LEDS-1)]);
  for (i=(NBR_LEDS-1); i>=0; i--) {
    COPY_COLOR(buffer[i], buffer[i-1]);
  }
  COPY_COLOR(buffer[0], temp);
}

/*******************  doubleSpinEffect  *****************************/
static void doubleSpinEffect(uint8_t buffer[][3], bool reset) {
  static uint8_t sub1[NBR_LEDS][3];
  static uint8_t sub2[NBR_LEDS][3];
  int i;
  static int step;

  if (reset) step = 0;

  whiteSpinEffect(sub1, reset);
  spinEffect2(sub2, reset);

  for (i=0; i<NBR_LEDS; i++)
  {
    ADD_COLOR(buffer[i], sub1[i], sub2[i]);
  }
  step ++;
}

/* LED-ring test effect */
#define TEST_INTENTS 20
static uint8_t test_pat[3][3] = {{TEST_INTENTS, 0, 0}, {0, TEST_INTENTS, 0}, {0, 0, TEST_INTENTS}};
static uint8_t test_eff_nbr = 0;

#define TEST_DELAY 4
static uint8_t test_delay_counter = 0;
static uint8_t headlight_test_counter =0;
static uint8_t test_front = false;

/**********  ledTestEffect  ********************/
static void ledTestEffect(uint8_t buffer[][3], bool reset)
{
  int i;
  static float brightness=0;

  if (reset) brightness = 0;

  if (brightness<1) brightness += 0.05f;
  else brightness = 1;

  for (i=0; i<NBR_LEDS; i++)
  {
    buffer[i][0] = test_pat[test_eff_nbr][0];
    buffer[i][1] = test_pat[test_eff_nbr][1];
    buffer[i][2] = test_pat[test_eff_nbr][2];
  }

  test_delay_counter++;
  headlight_test_counter++;

  if (test_delay_counter > TEST_DELAY) {
    test_delay_counter = 0;
    test_eff_nbr = (test_eff_nbr + 1) % 3;
  }

  if (headlight_test_counter > (TEST_DELAY*3)) {
    headlight_test_counter = 0;
    test_front = !test_front;
  }
}

/** An effect mimicking a blue light siren ***********************************/
static void siren(uint8_t buffer[][3], bool reset)
{
  int i;
  static int tic = 0;

  if (reset)
  {
    for (i=0; i<NBR_LEDS; i++) {
      COPY_COLOR(buffer[i], part_black);
    }
  }

  if ((tic < 10) && (tic & 1))
  {
    for (i=0; i<NBR_LEDS; i++) {
      COPY_COLOR(buffer[i], blue);
    }
  }
  else
  {
    for (i=0; i<NBR_LEDS; i++) {
      COPY_COLOR(buffer[i], part_black);
    }
  }
  if (++tic >= 20) tic = 0;
}


/**************** Effect list *********************************/
NeopixelRingEffect effectsFct[] = {blackEffect,      /* 0 */
                                   whiteSpinEffect,  /* 1 */
                                   solidColorEffect, /* 2 */
                                   boatEffect,       /* 3 */
                                   colorSpinEffect,  /* 4 */
                                   spinEffect2,      /* 5 */
                                   doubleSpinEffect, /* 6 */
                                   ledTestEffect,    /* 7 */
                                   siren,            /* 8 */
                                  };
 
/********** Ring init and switching **********/
/**
  * @brief  void NeopixelRingWorker(uint16_t user_effect)
  * @param  None
  * @retval None
  ******* Ring init and switching ***********
  */

void NeopixelRingWorker(void)
{
  static int current_effect = 0;
  static uint8_t buffer[NBR_LEDS][3];
  bool reset = true;

  if(new_effect)
  {
    reset = true;
    worker_state = 1;
    new_effect = 0;
  }  

  if(worker_state == 1)
  {
    if (effect > neffect)
    {
      ws2812Send(black, NBR_LEDS);  /* all leds off */
      return;
    }

    if (current_effect != effect)
    { 
      reset = 1;
    }
    else
    {
      reset = 0;
    }
    current_effect = user_effect;
    effectsFct[current_effect](buffer, reset);
    ws2812Send(buffer, NBR_LEDS);
    worker_state = 2;
  }
  else if (worker_state == 2)
  {
    reset = 0;
    current_effect = user_effect;
    effectsFct[current_effect](buffer, reset);
    ws2812Send(buffer, NBR_LEDS);
  }
}

/**
  * @brief  void NeopixelRingInit(void)
  * @param  None
  * @retval None
  */
void NeopixelRingInit(void)
{
  ws2812Init();
  neffect = sizeof(effectsFct)/sizeof(effectsFct[0])-1; /* 40/4 -1 = 9 */

  NeopixelRingTimer = RING_TIMER;
  NeopixelRingStart = false;  /* run NeoPixelRingWorker() from SysTick */
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void printNeopixelRingMenu(void)
{
  uint8_t count;

  for( count = 0; count<15; ++count)
  {
    usart2_printf(NeopixelRingMenu[count]);
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t NeopixelRingTest(void)
{
  static 	uint8_t user_wish;

  while(1)
  {
    clr_scr_rst_cur();
    printNeopixelRingMenu();

    user_wish = get_atoi();
    usart2_printf("%d 5",user_wish);

    if(user_wish == In_ESC)
    {
      return(0);
    }
    else if((user_wish >= 1) && user_wish <= 9)
    {
      if(!ws2812_init)
      {
        NeopixelRingInit();
      }

      while(!allLedDone)
      {
        /* Wait for previous transfer to be finished */
      }

      user_effect = user_wish - 1;
      NeopixelRingStart = true;
      new_effect = 1;          
    }

    else if(user_wish == 10)
    {
      NeopixelRingDeInit();
    }
    else
    {
     /* pritnf () */
    }
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void NeopixelRingDeInit(void)
{
  /* stop neopixelRing, reset neopixel leds & return */
  NeopixelRingStart = false;
  NeopixelRingTimer = 0;

  neffect = sizeof(effectsFct)/sizeof(effectsFct[0])-1;
  allLedDone = true;
  ws2812Send(black, NBR_LEDS);
  STM_EVAL_LEDOff(LED6); 

  TIM_Cmd(TIM3, DISABLE); 					// disable Timer 3
  DMA_Cmd(DMA1_Stream5, DISABLE); 	// disable DMA stream4

}


/******************************** End of File *********************************/

