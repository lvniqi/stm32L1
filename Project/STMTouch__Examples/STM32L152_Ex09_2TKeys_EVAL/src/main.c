/**
  ******************************************************************************
  * @file    STM32L152_Ex09_2TKeys_EVAL\src\main.c 
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

#define TEST_TKEY(NB) ((MyTKeysB[(NB)].p_Data->StateId == TSL_STATEID_DETECT) || \
                       (MyTKeysB[(NB)].p_Data->StateId == TSL_STATEID_DEB_RELEASE_DETECT))

/* PD0, PD1, PD2, PD5 */
// LEDs definition on STM32L152B-EVAL board
// PD0 = LED1
#define LED1_TOGGLE {GPIOD->ODR  ^= (1<<0);}
#define LED1_OFF    {GPIOD->BSRRL = (1<<0);}
#define LED1_ON     {GPIOD->BSRRH = (1<<0);}
// PD1 = LED2
#define LED2_TOGGLE {GPIOD->ODR  ^= (1<<1);}
#define LED2_OFF    {GPIOD->BSRRL = (1<<1);}
#define LED2_ON     {GPIOD->BSRRH = (1<<1);}
// PD4 = LED3
#define LED3_TOGGLE {GPIOD->ODR  ^= (1<<4);}
#define LED3_OFF    {GPIOD->BSRRL = (1<<4);}
#define LED3_ON     {GPIOD->BSRRH = (1<<4);}
// PD5 = LED4
#define LED4_TOGGLE {GPIOD->ODR  ^= (1<<5);}
#define LED4_OFF    {GPIOD->BSRRL = (1<<5);}
#define LED4_ON     {GPIOD->BSRRH = (1<<5);}

/* Private variables ---------------------------------------------------------*/

// For debug purpose with STMStudio
uint8_t DS[TSLPRM_TOTAL_TKEYS]; // To store the States (one per object)
int16_t DD[TSLPRM_TOTAL_TKEYS]; // To store the Deltas (one per channel)
uint16_t DM[TSLPRM_TOTAL_TKEYS];// To store the Measures (one per object)
uint16_t DR[TSLPRM_TOTAL_TKEYS];// To store the references (one per object)

/* Private functions prototype -----------------------------------------------*/
void Init_GPIOs(void);
void Init_LED_GPIOs(void);
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
  /* PD0, PD1, PD4, PD5 */
  
  /* Enables GPIOs clock */
  RCC->AHBENR |= RCC_AHBPeriph_GPIOD;
  
  GPIOD->BSRRL = (1<<0);
  GPIOD->OTYPER &= ~(1<<0); 
  GPIOD->MODER = (GPIOD->MODER & ~(1<<(2*0+1)))|(1<<(2*0));
  
  GPIOD->BSRRL = (1<<1);
  GPIOD->OTYPER &= ~(1<<1); 
  GPIOD->MODER = (GPIOD->MODER & ~(1<<(2*1+1)))|(1<<(2*1));
  
  GPIOD->BSRRL = (1<<4);
  GPIOD->OTYPER &= ~(1<<4); 
  GPIOD->MODER = (GPIOD->MODER & ~(1<<(2*4+1)))|(1<<(2*4));
  
  GPIOD->BSRRL = (1<<5);
  GPIOD->OTYPER &= ~(1<<5); 
  GPIOD->MODER = (GPIOD->MODER & ~(1<<(2*5+1)))|(1<<(2*5));
  
}


/**
  * @brief  Manage the activity on sensors when touched/released (example)
  * @param  None
  * @retval None
  */
void ProcessSensors(void)
{
  uint32_t idx = 0;
  uint32_t idx_ds = 0;
  uint32_t idx_dd = 0;
  uint32_t idx_dm = 0;
  uint32_t idx_dr = 0;

  // STMStudio debug  
  for (idx = 0; idx < TSLPRM_TOTAL_TKEYS; idx++)
  {
    DS[idx_ds++] = MyTKeysB[idx].p_Data->StateId;    
    DD[idx_dd++] = MyTKeysB[idx].p_ChD->Delta;
    DM[idx_dm++] = MyTKeysB[idx].p_ChD->Meas;
    DR[idx_dr++] = MyTKeysB[idx].p_ChD->Ref;
  }

  // TKEY 0
  if (TEST_TKEY(0))
  {
    LED1_ON;
  }
  else
  {
    LED1_OFF;
  }
  
  // TKEY 1
  if (TEST_TKEY(1))
  {
    LED2_ON;
  }
  else
  {
    LED2_OFF;
  }
  
}


/**
  * @brief  Executed when a sensor is in Error state
  * @param  None
  * @retval None
  */
void MyTKeys_ErrorStateProcess(void)
{
  // Add here your own processing when a sensor is in Error state
  TSL_tkey_SetStateOff();
}


/**
  * @brief  Executed when a sensor is in Off state
  * @param  None
  * @retval None
  */
void MyTKeys_OffStateProcess(void)
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
