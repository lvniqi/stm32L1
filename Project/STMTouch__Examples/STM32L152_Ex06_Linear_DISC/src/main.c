/**
  ******************************************************************************
  * @file    STM32L152_Ex06_Linear_DISC\src\main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    24-February-2014
  * @brief   Basic example of how to use the STMTouch Driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

// Put 0 to measure the memory footprint
#define APPLI_ENABLED 1

#define TEST_TKEY(NB) (MyTKeysB[(NB)].p_Data->StateId == TSL_STATEID_DETECT)

/* PB6, PB7 */
// LEDs definition on STM32L152B-DISC board
// PB6 = LED_GREEN
#define LED_GREEN_TOGGLE {GPIOB->ODR  ^= (1<<7);}
#define LED_GREEN_OFF    {GPIOB->BSRRL = (1<<7);}
#define LED_GREEN_ON     {GPIOB->BSRRH = (1<<7);}
// PB7 = LED_BLUE
#define LED_BLUE_TOGGLE {GPIOB->ODR  ^= (1<<6);}
#define LED_BLUE_OFF    {GPIOB->BSRRL = (1<<6);}
#define LED_BLUE_ON     {GPIOB->BSRRH = (1<<6);}

/* Macros used for set/reset bar LCD bar */
#define BAR0_ON  t_bar[1] |= 8
#define BAR0_OFF t_bar[1] &= ~8
#define BAR1_ON  t_bar[0] |= 8
#define BAR1_OFF t_bar[0] &= ~8
#define BAR2_ON  t_bar[1] |= 2
#define BAR2_OFF t_bar[1] &= ~2
#define BAR3_ON t_bar[0]  |= 2
#define BAR3_OFF t_bar[0] &= ~2

/* Private variables ---------------------------------------------------------*/
extern uint32_t process_sensor;
unsigned char Slider_Position[7];
extern uint8_t t_bar[2];

/* Private functions prototype -----------------------------------------------*/
void Init_GPIOs(void);
void Init_LED_GPIOs(void);
void LCD_GLASS_PreInit(void);
void ProcessSensors(void);

/**
  * @brief  Main routine.
  * @param  None
  * @retval None
  */

int main(void)
{
  //============================================================================
  // Init all GPIOs in AIN to reduce overall consumption (but SWD)
  //============================================================================

#if APPLI_ENABLED
  Init_GPIOs();
#endif
  
  
  //============================================================================
  // Init LED HW
  //============================================================================

#if APPLI_ENABLED
  Init_LED_GPIOs();
#endif
  
  
  //============================================================================
  // Init LCD glass
  //============================================================================

#if APPLI_ENABLED  
  LCD_GLASS_PreInit();
  LCD_GLASS_Init();
#endif
  
  //============================================================================
  // Init STMTouch driver
  //============================================================================  

  TSL_user_Init();
 
  //============================================================================
  // Main loop
  //============================================================================
  
  for (;;)
  {
    // Execute STMTouch Driver state machine
    if (TSL_user_Action() == TSL_STATUS_OK)
    {
#if APPLI_ENABLED      
      ProcessSensors(); // Execute sensors related tasks
#endif      
    }
    else
    {
      // Execute other tasks...
    }
  }
}

/**
  * @brief  Initializes GPIOs in AIN.
  * @param  None
  * @retval None
  */
void Init_GPIOs(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, ENABLE);

  
  /* set input analog modes for all IOs */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = 0x9FFF;// Let SWD pin default value
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = 0xFF0F;// Let Group 6 pin default value
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, DISABLE);
  
}

/**
  * @brief  Initializes the LED GPIOs.
  * @param  None
  * @retval None
  */
void Init_LED_GPIOs(void)
{
  /* PB6, PB7 */
  
  /* Enables GPIOs clock */
  RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
  
  GPIOB->BSRRL = (1<<6);
  GPIOB->OTYPER &= ~(1<<6); 
  GPIOB->MODER = (GPIOB->MODER & ~(1<<(2*6+1)))|(1<<(2*6));
  
  GPIOB->BSRRL = (1<<7);
  GPIOB->OTYPER &= ~(1<<7); 
  GPIOB->MODER = (GPIOB->MODER & ~(1<<(2*7+1)))|(1<<(2*7));
  
}

/**
  * @brief  LCD_GLASS_PreInit (clock + GPIOs).
  * @param  None
  * @retval None
  */
void LCD_GLASS_PreInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the GPIOs Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE);

  /* Enable  comparator clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_LCD | RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;



  /* Configure Output for LCD */
  /* Port A */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_LCD) ;

  /* Configure Output for LCD */
  /* Port B */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 \
                                | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_LCD) ;

  /* Configure Output for LCD */
  /* Port C*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 \
                                | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_LCD) ;
}

/**
  * @brief  Manage the activity on sensors when touched/released (example)
  * @param  None
  * @retval None
  */
void ProcessSensors(void)
{
  uint16_t Max_Value = 256/(9-TSLPRM_LINROT_RESOLUTION);
  
  if (!process_sensor) return;

  BAR0_OFF;
  BAR1_OFF;
  BAR2_OFF;
  BAR3_OFF;

  if (MyLinRots[0].p_Data->Position >= Max_Value/64)
  {
    BAR0_ON;
  }

  if (MyLinRots[0].p_Data->Position >= Max_Value/4)
  {
    BAR1_ON;
  }

  if (MyLinRots[0].p_Data->Position >= Max_Value/2)
  {
    BAR2_ON;
  }

  if (MyLinRots[0].p_Data->Position >= 3*Max_Value/4)
  {
    BAR3_ON;
  }

  
  /* Write Message */
  Slider_Position[3] = MyLinRots[0].p_Data->Position / 100;
  Slider_Position[4] = (MyLinRots[0].p_Data->Position / 10) % 10;
  Slider_Position[5] = MyLinRots[0].p_Data->Position % 10;

  Slider_Position[3] += 48;
  Slider_Position[4] += 48;
  Slider_Position[5] += 48;
  Slider_Position[0] = 'P';
  Slider_Position[1] = 'o';
  Slider_Position[2] = 's';
  LCD_GLASS_WriteChar(Slider_Position + 0, POINT_OFF, 0, 1);
  LCD_GLASS_WriteChar(Slider_Position + 1, POINT_OFF, 0, 2);
  LCD_GLASS_WriteChar(Slider_Position + 2, POINT_OFF, COLUMN_ON, 3);
  LCD_GLASS_WriteChar(Slider_Position + 3, 0, 0, 4);
  LCD_GLASS_WriteChar(Slider_Position + 4, 0, 0, 5);
  LCD_GLASS_WriteChar(Slider_Position + 5, 0, 0, 6);
}


/**
  * @brief  Executed when a sensor is in Error state
  * @param  None
  * @retval None
  */
void MyLinRots_ErrorStateProcess(void)
{
  // Add here your own processing when a sensor is in Error state
  TSL_linrot_SetStateOff();
}


/**
  * @brief  Executed when a sensor is in Off state
  * @param  None
  * @retval None
  */
void MyLinRots_OffStateProcess(void)
{
  // Add here your own processing when a sensor is in Off state
}


#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  for (;;)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
