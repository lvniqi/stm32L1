/**
  ******************************************************************************
  * @file    STM32L152_Ex02_Linear_EVAL\src\main.c 
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

/* PD3, PD7, PG14, PG15 */
// LEDs definition on STM32L152D-EVAL board
// PD3 = LED1
#define LED1_TOGGLE {GPIOD->ODR  ^= (1<<3);}
#define LED1_OFF    {GPIOD->BSRRL = (1<<3);}
#define LED1_ON     {GPIOD->BSRRH = (1<<3);}
// PD7 = LED2
#define LED2_TOGGLE {GPIOD->ODR  ^= (1<<7);}
#define LED2_OFF    {GPIOD->BSRRL = (1<<7);}
#define LED2_ON     {GPIOD->BSRRH = (1<<7);}
// PG14 = LED3
#define LED3_TOGGLE {GPIOG->ODR  ^= (1<<14);}
#define LED3_OFF    {GPIOG->BSRRL = (1<<14);}
#define LED3_ON     {GPIOG->BSRRH = (1<<14);}
// PG15 = LED4
#define LED4_TOGGLE {GPIOG->ODR  ^= (1<<15);}
#define LED4_OFF    {GPIOG->BSRRL = (1<<15);}
#define LED4_ON     {GPIOG->BSRRH = (1<<15);}

/* Private variables ---------------------------------------------------------*/

extern uint32_t process_sensor;

/* Private functions prototype -----------------------------------------------*/
void Init_GPIOs(void);
void Init_LED_GPIOs(void);
void Shield_To_GND(void);
TSL_Status_enum_T TSL_Action(void);
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
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF |
                        RCC_AHBPeriph_GPIOG |  RCC_AHBPeriph_GPIOH, ENABLE);

  
  /* set input analog modes for all IOs */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = 0x9FFF;// Let SWD pin default value
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = 0xFF0F;// Let Group 6 pin default value
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF |
                        RCC_AHBPeriph_GPIOG |  RCC_AHBPeriph_GPIOH, DISABLE);
  
}

/**
  * @brief  Initializes the LED GPIOs.
  * @param  None
  * @retval None
  */
void Init_LED_GPIOs(void)
{
  /* PD3, PD7, PG14, PG15 */
  
  /* Enables GPIOs clock */
  RCC->AHBENR |= RCC_AHBPeriph_GPIOD;
  RCC->AHBENR |= RCC_AHBPeriph_GPIOG;
  
  GPIOD->BSRRL = (1<<3);
  GPIOD->OTYPER &= ~(1<<3); 
  GPIOD->MODER = (GPIOD->MODER & ~(1<<(2*3+1)))|(1<<(2*3));

  GPIOD->BSRRL = (1<<7);
  GPIOD->OTYPER &= ~(1<<7); 
  GPIOD->MODER = (GPIOD->MODER & ~(1<<(2*7+1)))|(1<<(2*7));

  GPIOG->BSRRL = (1<<14);
  GPIOG->OTYPER &= ~(1<<14); 
  GPIOG->MODER = (GPIOG->MODER & ~(1<<(2*14+1)))|(1<<(2*14));

  GPIOG->BSRRL = (1<<15);
  GPIOG->OTYPER &= ~(1<<15); 
  GPIOG->MODER = (GPIOG->MODER & ~((uint32_t)1<<(2*15+1)))|(1<<(2*15));
  
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

  LED1_OFF;
  LED2_OFF;
  LED3_OFF;
  LED4_OFF;

  if (MyLinRots[0].p_Data->Position >= Max_Value/50)
  {
    LED1_ON;
  }

  if (MyLinRots[0].p_Data->Position >= Max_Value/4)
  {
    LED2_ON;
  }

  if (MyLinRots[0].p_Data->Position >= Max_Value/2)
  {
    LED3_ON;
  }

  if (MyLinRots[0].p_Data->Position >= 3*Max_Value/4)
  {
    LED4_ON;
  }
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
