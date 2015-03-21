/**
  ******************************************************************************
  * @file    STM32L152_Ex04_LPM_3TKeys_EVAL\src\tsl_user.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    24-February-2014
  * @brief   Touch-Sensing user configuration and api file.
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

#include "tsl_user.h"

// PG15 = LED4
#define LED4_TOGGLE {GPIOG->ODR  ^= (1<<15);}
#define LED4_OFF    {GPIOG->BSRRL = (1<<15);}
#define LED4_ON     {GPIOG->BSRRH = (1<<15);}

// STMStudio lock feature
#define TARGET_LOCK_ID 0 // Do not modify - shared with STMStudio host software
#define HOST_LOCK_ID   1 // Do not modify - shared with STMStudio host software

//==============================================================================
// Channels
//==============================================================================

// Channel Source and Configuration: Always placed in ROM
const TSL_ChannelSrc_T MyChannels_Src[TSLPRM_TOTAL_CHANNELS] = {
  { CHANNEL_0_SRC, CHANNEL_0_SAMPLE, CHANNEL_0_CHANNEL },
  { CHANNEL_1_SRC, CHANNEL_1_SAMPLE, CHANNEL_1_CHANNEL },
  { CHANNEL_2_SRC, CHANNEL_2_SAMPLE, CHANNEL_2_CHANNEL }
};

// Channel Destination: Always placed in ROM
const TSL_ChannelDest_T MyChannels_Dest[TSLPRM_TOTAL_CHANNELS] = {
  { CHANNEL_0_DEST },
  { CHANNEL_1_DEST },
  { CHANNEL_2_DEST }
};

// Channel Data: Always placed in RAM
TSL_ChannelData_T MyChannels_Data[TSLPRM_TOTAL_CHANNELS];

//------
// Banks
//------

// Always placed in ROM
CONST TSL_Bank_T MyBanks[TSLPRM_TOTAL_BANKS] = {
  {&MyChannels_Src[0], &MyChannels_Dest[0], MyChannels_Data, BANK_0_NBCHANNELS, BANK_0_SHIELD_SAMPLE, BANK_0_SHIELD_CHANNEL},
  {&MyChannels_Src[1], &MyChannels_Dest[1], MyChannels_Data, BANK_1_NBCHANNELS, BANK_1_SHIELD_SAMPLE, BANK_1_SHIELD_CHANNEL},
  {&MyChannels_Src[2], &MyChannels_Dest[2], MyChannels_Data, BANK_2_NBCHANNELS, BANK_2_SHIELD_SAMPLE, BANK_2_SHIELD_CHANNEL}   
};


//----------
// TouchKeys
//----------

// TouchKey Data: always placed in RAM (state, counter, flags, ...)
TSL_TouchKeyData_T MyTKeysData[TSLPRM_TOTAL_TKEYS];

// TouchKey Parameters: placed in RAM (thresholds, debounce counters, ...)
TSL_TouchKeyParam_T MyTKeysParams[TSLPRM_TOTAL_TKEYS];


// State Machine (ROM)

void MyTKeys_ErrorStateProcess(void);
void MyTKeys_OffStateProcess(void);

CONST TSL_State_T MyTKeys_StateMachine[] =
{
  // Calibration states
  /*  0 */ { TSL_STATEMASK_CALIB,              TSL_tkey_CalibrationStateProcess },
  /*  1 */ { TSL_STATEMASK_DEB_CALIB,          TSL_tkey_DebCalibrationStateProcess },
  // Release states 
  /*  2 */ { TSL_STATEMASK_RELEASE,            TSL_tkey_ReleaseStateProcess },
#if TSLPRM_USE_PROX > 0
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   TSL_tkey_DebReleaseProxStateProcess },
#else
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   0 },
#endif
  /*  4 */ { TSL_STATEMASK_DEB_RELEASE_DETECT, TSL_tkey_DebReleaseDetectStateProcess },
  /*  5 */ { TSL_STATEMASK_DEB_RELEASE_TOUCH,  TSL_tkey_DebReleaseTouchStateProcess },
#if TSLPRM_USE_PROX > 0
  // Proximity states
  /*  6 */ { TSL_STATEMASK_PROX,               TSL_tkey_ProxStateProcess },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           TSL_tkey_DebProxStateProcess },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    TSL_tkey_DebProxDetectStateProcess },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     TSL_tkey_DebProxTouchStateProcess },  
#else
  /*  6 */ { TSL_STATEMASK_PROX,               0 },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           0 },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    0 },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     0 },  
#endif
  // Detect states
  /* 10 */ { TSL_STATEMASK_DETECT,             TSL_tkey_DetectStateProcess },
  /* 11 */ { TSL_STATEMASK_DEB_DETECT,         TSL_tkey_DebDetectStateProcess },
  // Touch state
  /* 12 */ { TSL_STATEMASK_TOUCH,              TSL_tkey_TouchStateProcess },
  // Error states
  /* 13 */ { TSL_STATEMASK_ERROR,              MyTKeys_ErrorStateProcess },
  /* 14 */ { TSL_STATEMASK_DEB_ERROR_CALIB,    TSL_tkey_DebErrorStateProcess },
  /* 15 */ { TSL_STATEMASK_DEB_ERROR_RELEASE,  TSL_tkey_DebErrorStateProcess },
  /* 16 */ { TSL_STATEMASK_DEB_ERROR_PROX,     TSL_tkey_DebErrorStateProcess },
  /* 17 */ { TSL_STATEMASK_DEB_ERROR_DETECT,   TSL_tkey_DebErrorStateProcess },
  /* 18 */ { TSL_STATEMASK_DEB_ERROR_TOUCH,    TSL_tkey_DebErrorStateProcess },
  // Other states
  /* 19 */ { TSL_STATEMASK_OFF,                MyTKeys_OffStateProcess }
};

CONST TSL_TouchKeyMethods_T MyTKeys_Methods =
{
  TSL_tkey_Init,
  TSL_tkey_Process
};

// TouchKey Data: always placed in RAM (state, counter, flags, ...)
TSL_TouchKeyData_T MyTKey0Data;
TSL_TouchKeyData_T MyTKey1Data;
TSL_TouchKeyData_T MyTKey2Data;

// TouchKey Parameters: placed in RAM (thresholds, debounce counters, ...)
TSL_TouchKeyParam_T MyTKey0Params;
TSL_TouchKeyParam_T MyTKey1Params;
TSL_TouchKeyParam_T MyTKey2Params;

// TouchKeys: Always placed in ROM

const TSL_TouchKeyB_T MyTKeysB[3] =
{
  { &MyTKey0Data, &MyTKey0Params, &MyChannels_Data[CHANNEL_0_DEST]},
  { &MyTKey1Data, &MyTKey1Params, &MyChannels_Data[CHANNEL_1_DEST]},
  { &MyTKey2Data, &MyTKey2Params, &MyChannels_Data[CHANNEL_2_DEST]}
};

//----------------
// Generic Objects
//----------------

// Objects: Always placed in ROM
CONST TSL_Object_T MyObjects[TSLPRM_TOTAL_OBJECTS] =
{
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKeyB_T *)&MyTKeysB[0] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKeyB_T *)&MyTKeysB[1] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKeyB_T *)&MyTKeysB[2] },
};

// Group of objects: placed in RAM due to state variables
TSL_ObjectGroup_T MyObjGroup = { MyObjects, 3, 0x00, TSL_STATE_NOT_CHANGED };

//-------------------------------------------
// TSL Common Parameters placed in RAM or ROM
// --> external declaration in tsl_conf.h
//-------------------------------------------

TSL_Params_T TSL_Params =
{
  TSLPRM_ACQ_MIN,
  TSLPRM_ACQ_MAX,
  TSLPRM_CALIB_SAMPLES,
  TSLPRM_DTO,
  MyTKeys_StateMachine, // Default TKeys state machine
  &MyTKeys_Methods
};

// STMStudio lock feature
typedef struct petersons_t {
    volatile unsigned char flag[2]; // Do not modify - shared with STMStudio host software
    volatile unsigned char turn;    // Do not modify - shared with STMStudio host software
} petersons_t;

/* Private functions prototype -----------------------------------------------*/

void TSL_user_InitGPIOs(void);
void TSL_user_SetThresholds(void);

/* Global variables ----------------------------------------------------------*/

TSL_tTick_ms_T ECS_last_tick; // Hold the last time value for ECS

// stm_studio_lock symbol used by the STMStudio host software for synchronization
petersons_t stm_studio_lock = { { 0, 0 }, TARGET_LOCK_ID }; // Do not modify - shared with STMStudio host software

/**
  * @brief  Initialize the STMTouch Driver
  * @param  None
  * @retval None
  */
void TSL_user_Init(void)
{
#if TSLPRM_USE_SHIELD == 0
  TSL_user_InitGPIOs();
#endif

  TSL_obj_GroupInit(&MyObjGroup); // Init Objects
  
  TSL_Init(MyBanks); // Init timing and acquisition modules
  
  TSL_user_SetThresholds(); // Init thresholds for each object individually
}


/**
  * @brief  Execute STMTouch Driver main State machine
  * @param  None
  * @retval status Return TSL_STATUS_OK if the acquisition is done
  */
TSL_Status_enum_T TSL_user_Action(void)
{
  static uint8_t idx_bank=0;
  static uint8_t ConfigDone=0;
  TSL_Status_enum_T status = TSL_STATUS_BUSY;
  
  if(!ConfigDone)
  {
    // Configure Bank
    TSL_acq_BankConfig(idx_bank);
 
    // Start Bank acquisition
    TSL_acq_BankStartAcq();
    
    // Set flag
    ConfigDone=1;
    
  }
  
  // Check Bank End of Acquisition, before entering sleep mode
  if (TSL_acq_BankWaitEOC() != TSL_STATUS_OK)
  {
    // enter low power sleep
		// Disable GPIOs clock used
    //RCC->AHBENR &=~ (RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOFEN | RCC_AHBENR_GPIOGEN);
		RCC->AHBENR &=~ (RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOGEN);
		RCC->AHBENR &=~ (RCC_AHBENR_GPIOBEN);
		//RCC->AHBENR &=~ (RCC_AHBENR_GPIOFEN);
    //shutdown systick
    SysTick->CTRL  &=~ SysTick_CTRL_ENABLE_Msk; 
    // Enable The power down mode during Sleep mode
    FLASH_SLEEPPowerDownCmd(ENABLE);
    // Request to enter SLEEP mode with regulator in low power mode
    PWR_EnterSleepMode(PWR_Regulator_LowPower, PWR_SLEEPEntry_WFI);
    //enable systick
    SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;
   // Enable GPIOs clock used
    RCC->AHBENR |= (RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOFEN | RCC_AHBENR_GPIOGEN);
  }
  
  // Re-check Bank End of Acquisition, after exiting sleep mode
  if (TSL_acq_BankWaitEOC() == TSL_STATUS_OK)
  {
    // Get Bank Result
    STMSTUDIO_LOCK;
    TSL_acq_BankGetResult(idx_bank, 0, 0);
    STMSTUDIO_UNLOCK;
    ConfigDone=0;
    idx_bank++;
  
    // Check all of the banks are acquired
    if(idx_bank == TSLPRM_TOTAL_BANKS)
    {
      idx_bank=0;
    
      // Process Objects
      TSL_obj_GroupProcess(&MyObjGroup);
  
      // DxS processing
      TSL_dxs_FirstObj(&MyObjGroup);

        
      status = TSL_STATUS_OK; // All banks have been acquired and sensors processed
    
      /* Low Power while no activities on touch keys */
      if((&MyObjGroup)->StateMask == TSL_STATE_RELEASE_BIT_MASK)
      {
        StopRTCLSIMode();
        TSL_ecs_Process(&MyObjGroup);
      }
    }
  }
  else
  {
    status = TSL_STATUS_BUSY;
  }
  
  return status;
}

/**
  * @brief  Initializes the TouchSensing GPIOs.
  * @param  None
  * @retval None
  */
void TSL_user_InitGPIOs(void)
{
  // Configure the Shield IO (PB6) to ground when not used.
  
  // Enables GPIOs clock
  RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
  
  GPIOB->BSRRH = (1<<6);
  GPIOB->OTYPER &= ~(1<<6); 
  GPIOB->MODER = (GPIOB->MODER & ~(1<<(2*6+1)))|(1<<(2*6));
}


/**
  * @brief  Set thresholds for each object (optional).
  * @param  None
  * @retval None
  */
void TSL_user_SetThresholds(void)
{
  // Example: Decrease the Detect thresholds for the TKEY 0
  //MyTKeys_Param[0].DetectInTh -= 10;
  //MyTKeys_Param[0].DetectOutTh -= 10;
  
  //K0 parameter modification
#if TSLPRM_USE_PROX >1  
  MyTKeysB[0].p_Param->ProxInTh=10;
  MyTKeysB[0].p_Param->ProxOutTh=5; 
#endif  
  MyTKeysB[0].p_Param->DetectInTh=20;
  MyTKeysB[0].p_Param->DetectOutTh=15;
  MyTKeysB[0].p_Param->CalibTh=20;
}

/**
  * @brief  To call before modifying any critical data. In case the host is 
            inside or asked for entering the critical section, this routine 
            will wait for the host to leave the critical section.
  * @param  None
  * @retval None
  */
void enterLock (void) {
    stm_studio_lock.flag[TARGET_LOCK_ID] = 1;
    stm_studio_lock.turn = HOST_LOCK_ID;
    while (stm_studio_lock.flag[HOST_LOCK_ID] && (stm_studio_lock.turn == HOST_LOCK_ID)) {}
}

/**
  * @brief  Leave the critical section. If the host is waiting, access will be 
            granted to him. Otherwise the first next one asking will own the turn.
  * @param  None
  * @retval None
  */
void exitLock (void) {
    stm_studio_lock.flag[TARGET_LOCK_ID] = 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
