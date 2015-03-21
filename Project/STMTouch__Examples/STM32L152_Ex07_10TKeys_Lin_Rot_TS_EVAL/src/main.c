/**
  ******************************************************************************
  * @file    STM32L152_Ex07_10TKeys_Lin_Rot_TS_EVAL\src\main.c 
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

#define MESSAGE_TITLE_1 " STM32 TS Lib  "
#define MESSAGE_LINEAR  " LIN pos: ---  "
#define MESSAGE_ROTARY  " ROT pos: ---  "
#define MESSAGE_K1K2K3  "   K1  K2  K3  "
#define MESSAGE_K4K5K6  "   K4  K5  K6  "
#define MESSAGE_K7K8K9  "   K7  K8  K9  "
#define MESSAGE_K10     "       K10     "

#define LCD_LINE_TITLE1  LCD_LINE_0
#define LCD_LINE_TITLE2  LCD_LINE_1
#define LCD_LINE_TITLE3  LCD_LINE_2
#define LCD_LINE_TITLE4  LCD_LINE_3
#define LCD_LINE_TITLE5  LCD_LINE_4
#define LCD_LINE_LINEAR  LCD_LINE_6
#define LCD_LINE_ROTARY  LCD_LINE_7
#define LCD_LINE_K1K2K3  LCD_LINE_8
#define LCD_LINE_K4K5K6  LCD_LINE_9
#define LCD_LINE_K7K8K9  LCD_LINE_10
#define LCD_LINE_K10     LCD_LINE_11

/* Private variables ---------------------------------------------------------*/

// For debug purpose with STMStudio
uint8_t DS_TK[TSLPRM_TOTAL_TKEYS]; // To store the States (one per object)
int16_t DD_TK[TSLPRM_TOTAL_TKEYS]; // To store the Deltas (one per channel)
uint16_t DM_TK[TSLPRM_TOTAL_TKEYS];// To store the Measures (one per object)
uint16_t DR_TK[TSLPRM_TOTAL_TKEYS];// To store the references (one per object)
uint32_t DV_MK[TSLPRM_TOTAL_LNRTS];// To store the values for multi-keys (one per object) 
TSL_tDelta_T DD_MK_LIN[3];
TSL_tDelta_T DD_MK_ROT[3];

/* TKeys order: K2,K3,K9,K10,K1,K6,K4,K5,K7,K8 */
uint16_t TKeyLine_LookUp[TSLPRM_TOTAL_TKEYS]={192,192,
                                              240,264,
                                              192,216,
                                              216,216,
                                              240,240};

uint8_t TKeyColumn_LookUp[TSLPRM_TOTAL_TKEYS]={0x90,0x50,0x50,0x90,0xD0,0x50,0xD0,0x90,0xD0,0x90};


/* Private functions prototype -----------------------------------------------*/
void Init_GPIOs(void);
void ProcessSensors(void);
void ManageScreen(void);

/**
  * @brief  Main routine.
  * @param  None
  * @retval None
  */
int main(void)
{
#if APPLI_ENABLED  
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  //============================================================================
  // Init all GPIOs in AIN to reduce overall consumption (but SWD)
  //============================================================================

  Init_GPIOs();
  
  //============================================================================
  // Init LCD
  //============================================================================
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* LCD nRST */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);

  /* LCD back light */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);

  /* Initialize the LCD */
  STM32L152_TS_LCD_Init();

  /* Clean the LCD */
  LCD_Clear(LCD_COLOR_BLUE);

  /* Set default colors */
  LCD_SetColors(LCD_COLOR_GREEN, LCD_COLOR_BLUE);

  /* Write Messages */
  LCD_DisplayStringLine(LCD_LINE_TITLE1, (uint8_t*)MESSAGE_TITLE_1);
  LCD_DisplayStringLine(LCD_LINE_ROTARY, (uint8_t*)MESSAGE_ROTARY);
  LCD_DisplayStringLine(LCD_LINE_LINEAR, (uint8_t*)MESSAGE_LINEAR);
  LCD_DisplayStringLine(LCD_LINE_K1K2K3, (uint8_t*)MESSAGE_K1K2K3);
  LCD_DisplayStringLine(LCD_LINE_K4K5K6, (uint8_t*)MESSAGE_K4K5K6);
  LCD_DisplayStringLine(LCD_LINE_K7K8K9, (uint8_t*)MESSAGE_K7K8K9);
  LCD_DisplayStringLine(LCD_LINE_K10, (uint8_t*)MESSAGE_K10);
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

#if APPLI_ENABLED      
    ManageScreen(); // Execute screen related tasks
#endif 

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
  TSL_ChannelData_T  *p_ChD;
  
  for (idx = 0; idx < TSLPRM_TOTAL_TKEYS; idx++)
  {
    // Write datas for TKeys
    DS_TK[idx_ds++] = MyTKeysB[idx].p_Data->StateId;    
    DD_TK[idx_dd++] = MyTKeysB[idx].p_ChD->Delta;
    DM_TK[idx_dm++] = MyTKeysB[idx].p_ChD->Meas;
    DR_TK[idx_dr++] = MyTKeysB[idx].p_ChD->Ref;
    
    if (TSL_tkey_GetStateId() == TSL_STATEID_ERROR)
    {
      TSL_tkey_SetStateOff();
    }
  }
  
  for (idx = 0; idx < TSLPRM_TOTAL_LNRTS; idx++)
  {
    //Write datas for linear and rotary sensors
    DV_MK[idx] = MyLinRots[idx].p_Data->Position;
  }
  
  p_ChD = MyLinRots[0].p_ChD;
  DD_MK_LIN[0] = p_ChD->Delta;
  p_ChD++;
  DD_MK_LIN[1] = p_ChD->Delta;
  p_ChD++;
  DD_MK_LIN[2] = p_ChD->Delta;
  
  p_ChD = MyLinRots[1].p_ChD;
  DD_MK_ROT[0] = p_ChD->Delta;
  p_ChD++;
  DD_MK_ROT[1] = p_ChD->Delta;
  p_ChD++;
  DD_MK_ROT[2] = p_ChD->Delta;

}


/**
  * @brief  Manage screen when touch
  * @param  None
  * @retval None
  */
void ManageScreen(void)
{
  uint32_t idx = 0;
  uint8_t CharToDraw = ' ';
  uint8_t LinRotPos[]={'-','-','-'};
  static uint32_t Previous_DS_TK[TSLPRM_TOTAL_TKEYS];
  static uint32_t Previous_DV_MK[TSLPRM_TOTAL_LNRTS];

  /* Manage TouchKeys */
  for(idx=0; idx<TSLPRM_TOTAL_TKEYS; idx++)
  {
    if(Previous_DS_TK[idx]!=DS_TK[idx])
    {
      Previous_DS_TK[idx]=DS_TK[idx]; 
      if((DS_TK[idx]==TSL_STATEID_DETECT)||(DS_TK[idx]==TSL_STATEID_DEB_RELEASE_DETECT))
      {
        CharToDraw='^';
      }
      else
      {
        CharToDraw=' ';
      }
      LCD_DisplayChar(TKeyLine_LookUp[idx], TKeyColumn_LookUp[idx], CharToDraw);
    }
  }

 
  for(idx=0; idx<TSLPRM_TOTAL_LNRTS; idx++)
  {
    if(Previous_DV_MK[idx]!=DV_MK[idx])
    {
      Previous_DV_MK[idx]=DV_MK[idx];
      
      LinRotPos[0] = DV_MK[idx] / 100;
      LinRotPos[1] = (DV_MK[idx]  / 10) % 10;
      LinRotPos[2] = DV_MK[idx]  % 10;
      
      LinRotPos[0] += 48;
      LinRotPos[1] += 48;
      LinRotPos[2] += 48;
  
      LCD_DisplayChar(144+24*idx, 0x50, LinRotPos[0]);
      LCD_DisplayChar(144+24*idx, 0x40, LinRotPos[1]);
      LCD_DisplayChar(144+24*idx, 0x30, LinRotPos[2]);
    }
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
