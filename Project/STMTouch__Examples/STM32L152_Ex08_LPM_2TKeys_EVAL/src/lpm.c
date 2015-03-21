/**
  ******************************************************************************
  * @file    STM32L152_Ex08_LPM_2TKeys_EVAL\src\lpm.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    24-February-2014
  * @brief   Low Power Mode user configuration and api file.
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
#include "lpm.h"

/* Private typedefs ----------------------------------------------------------*/
typedef struct
{
  uint32_t MODER;
  uint32_t OSPEEDR;
  uint16_t OTYPER;
  uint32_t PUPDR;
}GPIO_ConfigRegisterTypeDef;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private functions prototype -----------------------------------------------*/
void GPIO_GetConfig(GPIO_TypeDef* GPIOx, GPIO_ConfigRegisterTypeDef* GPIO_ConfigRegister);
void GPIO_SetConfig(GPIO_TypeDef* GPIOx, GPIO_ConfigRegisterTypeDef* GPIO_ConfigRegister);


/**
  * @brief  Configures the RTC Wakeup.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
  EXTI_InitTypeDef  EXTI_InitStructure;

  /* Enable PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset RTC Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCCLKCmd(DISABLE);
  RCC_RTCResetCmd(DISABLE);
    
  /*!< LSI Enable */
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /* RTC Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* EXTI configuration *******************************************************/
  EXTI_ClearITPendingBit(EXTI_Line20);
  EXTI_InitStructure.EXTI_Line = EXTI_Line20;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  
  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16, Wakeup Time Base: ~4s 
     RTC Clock Source LSI ~37KHz  
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(0x10A);  // 125 ms

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
      
  /* Enable Ultra low power mode */
  PWR_UltraLowPowerCmd(ENABLE);
}

/**
  * @brief  GPIO_GetConfig.
  * @param[in]  GPIOx
  * @param[out]  GPIO_ConfigRegister
  * @retval None
  */
void GPIO_GetConfig(GPIO_TypeDef* GPIOx, GPIO_ConfigRegisterTypeDef* GPIO_ConfigRegister)
{
  GPIO_ConfigRegister->MODER = GPIOx->MODER;
  GPIO_ConfigRegister->OSPEEDR = GPIOx->OSPEEDR;
  GPIO_ConfigRegister->OTYPER = GPIOx->OTYPER;
  GPIO_ConfigRegister->PUPDR = GPIOx->PUPDR;
}

/**
  * @brief  GPIO_SetConfig.
  * @param[in]  GPIOx
  * @param[in]  GPIO_ConfigRegister
  * @retval None
  */
void GPIO_SetConfig(GPIO_TypeDef* GPIOx, GPIO_ConfigRegisterTypeDef* GPIO_ConfigRegister)
{
  GPIOx->OSPEEDR = GPIO_ConfigRegister->OSPEEDR;
  GPIOx->OTYPER = GPIO_ConfigRegister->OTYPER;
  GPIOx->PUPDR = GPIO_ConfigRegister->PUPDR;
  GPIOx->MODER = GPIO_ConfigRegister->MODER;
}

/**
  * @brief  StopRTCLSIMode_Measure.
  * @param  None
  * @retval None
  */
void StopRTCLSIMode(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_ConfigRegisterTypeDef GPIOA_ConfigRegister;
  GPIO_ConfigRegisterTypeDef GPIOB_ConfigRegister;
  GPIO_ConfigRegisterTypeDef GPIOC_ConfigRegister;
  GPIO_ConfigRegisterTypeDef GPIOD_ConfigRegister;
  GPIO_ConfigRegisterTypeDef GPIOE_ConfigRegister;
  GPIO_ConfigRegisterTypeDef GPIOH_ConfigRegister;

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, ENABLE);

  
  /* Get IOs config */
  GPIO_GetConfig(GPIOA, &GPIOA_ConfigRegister);
  GPIO_GetConfig(GPIOB, &GPIOB_ConfigRegister);
  GPIO_GetConfig(GPIOC, &GPIOC_ConfigRegister);
  GPIO_GetConfig(GPIOD, &GPIOD_ConfigRegister);
  GPIO_GetConfig(GPIOE, &GPIOE_ConfigRegister);
  GPIO_GetConfig(GPIOH, &GPIOH_ConfigRegister);

  /* set input analog modes for all IOs */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = 0xFF1F;// better to do like this due to the analog switch which will be closed in AIN mode
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    
  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE);
    
  /* Enter Stop Mode */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);

  /* Clear flags after wake up */
  RTC_ClearITPendingBit(RTC_IT_WUT);
  EXTI_ClearITPendingBit(EXTI_Line20);
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Disable Wakeup Counter */
  RTC_WakeUpCmd(DISABLE);
  
    
  /* After wake-up from STOP reconfigure the system clock */
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {}

  /* Enable PLL */
  RCC_PLLCmd(ENABLE);
    
  /* Wait till PLL is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {}
    
  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x0C)
  {}

  /* Set IOs config */
  GPIO_SetConfig(GPIOA, &GPIOA_ConfigRegister);
  GPIO_SetConfig(GPIOB, &GPIOB_ConfigRegister);
  GPIO_SetConfig(GPIOC, &GPIOC_ConfigRegister);
  GPIO_SetConfig(GPIOD, &GPIOD_ConfigRegister);
  GPIO_SetConfig(GPIOE, &GPIOE_ConfigRegister);
  GPIO_SetConfig(GPIOH, &GPIOH_ConfigRegister);

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
