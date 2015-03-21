/**
  ******************************************************************************
  * @file    STM32L152_Ex03_10TKeys_Lin_Rot_TS_EVAL\src\tsl_user.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    24-February-2014
  * @brief   Touch-Sensing Application configuration file.
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
  { CHANNEL_2_SRC, CHANNEL_2_SAMPLE, CHANNEL_2_CHANNEL },
  { CHANNEL_3_SRC, CHANNEL_3_SAMPLE, CHANNEL_3_CHANNEL },
  { CHANNEL_4_SRC, CHANNEL_4_SAMPLE, CHANNEL_4_CHANNEL },
  { CHANNEL_5_SRC, CHANNEL_5_SAMPLE, CHANNEL_5_CHANNEL },
  { CHANNEL_6_SRC, CHANNEL_6_SAMPLE, CHANNEL_6_CHANNEL },
  { CHANNEL_7_SRC, CHANNEL_7_SAMPLE, CHANNEL_7_CHANNEL },
  { CHANNEL_8_SRC, CHANNEL_8_SAMPLE, CHANNEL_8_CHANNEL },
  { CHANNEL_9_SRC, CHANNEL_9_SAMPLE, CHANNEL_9_CHANNEL },
  { CHANNEL_10_SRC, CHANNEL_10_SAMPLE, CHANNEL_10_CHANNEL },
  { CHANNEL_11_SRC, CHANNEL_11_SAMPLE, CHANNEL_11_CHANNEL },
  { CHANNEL_12_SRC, CHANNEL_12_SAMPLE, CHANNEL_12_CHANNEL },
  { CHANNEL_13_SRC, CHANNEL_13_SAMPLE, CHANNEL_13_CHANNEL },
  { CHANNEL_14_SRC, CHANNEL_14_SAMPLE, CHANNEL_14_CHANNEL },
  { CHANNEL_15_SRC, CHANNEL_15_SAMPLE, CHANNEL_15_CHANNEL }
};

// Channel Destination: Always placed in ROM
const TSL_ChannelDest_T MyChannels_Dest[TSLPRM_TOTAL_CHANNELS] = {
  { CHANNEL_0_DEST },
  { CHANNEL_1_DEST },
  { CHANNEL_2_DEST },
  { CHANNEL_3_DEST },
  { CHANNEL_4_DEST },
  { CHANNEL_5_DEST },
  { CHANNEL_6_DEST },
  { CHANNEL_7_DEST },
  { CHANNEL_8_DEST },
  { CHANNEL_9_DEST },
  { CHANNEL_10_DEST },
  { CHANNEL_11_DEST },
  { CHANNEL_12_DEST },
  { CHANNEL_13_DEST },
  { CHANNEL_14_DEST },
  { CHANNEL_15_DEST }
};

// Channel Data: Always placed in RAM
TSL_ChannelData_T MyChannels_Data[TSLPRM_TOTAL_CHANNELS];

//------
// Banks
//------

// Always placed in ROM
CONST TSL_Bank_T MyBanks[TSLPRM_TOTAL_BANKS] = {
  {&MyChannels_Src[0], &MyChannels_Dest[0], MyChannels_Data, BANK_0_NBCHANNELS, BANK_0_SHIELD_SAMPLE, BANK_0_SHIELD_CHANNEL},
  {&MyChannels_Src[7], &MyChannels_Dest[7], MyChannels_Data, BANK_1_NBCHANNELS, BANK_1_SHIELD_SAMPLE, BANK_1_SHIELD_CHANNEL},
  {&MyChannels_Src[12], &MyChannels_Dest[12], MyChannels_Data, BANK_2_NBCHANNELS, BANK_2_SHIELD_SAMPLE, BANK_2_SHIELD_CHANNEL}   
  
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
TSL_TouchKeyData_T MyTKey3Data;
TSL_TouchKeyData_T MyTKey4Data;
TSL_TouchKeyData_T MyTKey5Data;
TSL_TouchKeyData_T MyTKey6Data;
TSL_TouchKeyData_T MyTKey7Data;
TSL_TouchKeyData_T MyTKey8Data;
TSL_TouchKeyData_T MyTKey9Data;

// TouchKey Parameters: placed in RAM (thresholds, debounce counters, ...)
TSL_TouchKeyParam_T MyTKey0Params;
TSL_TouchKeyParam_T MyTKey1Params;
TSL_TouchKeyParam_T MyTKey2Params;
TSL_TouchKeyParam_T MyTKey3Params;
TSL_TouchKeyParam_T MyTKey4Params;
TSL_TouchKeyParam_T MyTKey5Params;
TSL_TouchKeyParam_T MyTKey6Params;
TSL_TouchKeyParam_T MyTKey7Params;
TSL_TouchKeyParam_T MyTKey8Params;
TSL_TouchKeyParam_T MyTKey9Params;

// TouchKeys: Always placed in ROM

const TSL_TouchKeyB_T MyTKeysB[10] =
{
  { &MyTKey0Data, &MyTKey0Params, &MyChannels_Data[CHANNEL_0_DEST]},
  { &MyTKey1Data, &MyTKey1Params, &MyChannels_Data[CHANNEL_1_DEST]},
  { &MyTKey2Data, &MyTKey2Params, &MyChannels_Data[CHANNEL_2_DEST]},
  { &MyTKey3Data, &MyTKey3Params, &MyChannels_Data[CHANNEL_3_DEST]},
  { &MyTKey4Data, &MyTKey4Params, &MyChannels_Data[CHANNEL_7_DEST]},
  { &MyTKey5Data, &MyTKey5Params, &MyChannels_Data[CHANNEL_8_DEST]},
  { &MyTKey6Data, &MyTKey6Params, &MyChannels_Data[CHANNEL_12_DEST]},
  { &MyTKey7Data, &MyTKey7Params, &MyChannels_Data[CHANNEL_13_DEST]},
  { &MyTKey8Data, &MyTKey8Params, &MyChannels_Data[CHANNEL_14_DEST]},
  { &MyTKey9Data, &MyTKey9Params, &MyChannels_Data[CHANNEL_15_DEST]},
};

//==============================================================================
// Linear and Rotary sensors
//==============================================================================

// Data (RAM)
TSL_LinRotData_T MyLinRots_Data[TSLPRM_TOTAL_LINROTS];

// Parameters (RAM)
TSL_LinRotParam_T MyLinRots_Param[TSLPRM_TOTAL_LINROTS];

// State Machine (ROM)

CONST TSL_State_T MyLinRots_StateMachine[] =
{
  // Calibration states
  /*  0 */ { TSL_STATEMASK_CALIB,              TSL_linrot_CalibrationStateProcess },
  /*  1 */ { TSL_STATEMASK_DEB_CALIB,          TSL_linrot_DebCalibrationStateProcess },
  // Release states 
  /*  2 */ { TSL_STATEMASK_RELEASE,            TSL_linrot_ReleaseStateProcess },
#if TSLPRM_USE_PROX > 0
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   TSL_linrot_DebReleaseProxStateProcess },
#else
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   0 },
#endif
  /*  4 */ { TSL_STATEMASK_DEB_RELEASE_DETECT, TSL_linrot_DebReleaseDetectStateProcess },
  /*  5 */ { TSL_STATEMASK_DEB_RELEASE_TOUCH,  TSL_linrot_DebReleaseTouchStateProcess },
#if TSLPRM_USE_PROX > 0
  // Proximity states
  /*  6 */ { TSL_STATEMASK_PROX,               TSL_linrot_ProxStateProcess },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           TSL_linrot_DebProxStateProcess },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    TSL_linrot_DebProxDetectStateProcess },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     TSL_linrot_DebProxTouchStateProcess },
#else
  /*  6 */ { TSL_STATEMASK_PROX,               0 },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           0 },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    0 },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     0 },
#endif
  // Detect states
  /* 10 */ { TSL_STATEMASK_DETECT,             TSL_linrot_DetectStateProcess },
  /* 11 */ { TSL_STATEMASK_DEB_DETECT,         TSL_linrot_DebDetectStateProcess },
  // Touch state
  /* 12 */ { TSL_STATEMASK_TOUCH,              TSL_linrot_TouchStateProcess },
  // Error states
  /* 13 */ { TSL_STATEMASK_ERROR,              MyLinRots_ErrorStateProcess },
  /* 14 */ { TSL_STATEMASK_DEB_ERROR_CALIB,    TSL_linrot_DebErrorStateProcess },
  /* 15 */ { TSL_STATEMASK_DEB_ERROR_RELEASE,  TSL_linrot_DebErrorStateProcess },
  /* 16 */ { TSL_STATEMASK_DEB_ERROR_PROX,     TSL_linrot_DebErrorStateProcess },
  /* 17 */ { TSL_STATEMASK_DEB_ERROR_DETECT,   TSL_linrot_DebErrorStateProcess },
  /* 18 */ { TSL_STATEMASK_DEB_ERROR_TOUCH,    TSL_linrot_DebErrorStateProcess },
  // Other states
  /* 19 */ { TSL_STATEMASK_OFF,                MyLinRots_OffStateProcess }
};

// Methods for "extended" type (ROM)
CONST TSL_LinRotMethods_T MyLinRots_Methods =
{
  TSL_linrot_Init,
  TSL_linrot_Process,
  TSL_linrot_CalcPos
};

// Delta Normalization Process
// The MSB is the integer part, the LSB is the real part
// Examples:
// - To apply a factor 1.10:
//   0x01 to the MSB
//   0x1A to the LSB (0.10 x 256 = 25.6 -> rounded to 26 = 0x1A)
// - To apply a factor 0.90:
//   0x00 to the MSB
//   0xE6 to the LSB (0.90 x 256 = 230.4 -> rounded to 230 = 0xE6)
CONST uint16_t MyLinRot0_DeltaCoeff[3] = {0x0259, 0x013D, 0x0100};
CONST uint16_t MyLinRot1_DeltaCoeff[3] = {0x0157, 0x0200, 0x0126};

// LinRots list (ROM)
CONST TSL_LinRot_T MyLinRots[TSLPRM_TOTAL_LINROTS] =
{
  { 
    &MyLinRots_Data[0], 
    &MyLinRots_Param[0], 
    &MyChannels_Data[CHANNEL_4_DEST], 
    3, //number of channels
    MyLinRot0_DeltaCoeff,
    (TSL_tsignPosition_T *)TSL_POSOFF_3CH_LIN_H,
    TSL_SCTCOMP_3CH_LIN_H,
    TSL_POSCORR_3CH_LIN_H,
    MyLinRots_StateMachine, 
    &MyLinRots_Methods
  },
  { 
    &MyLinRots_Data[1], 
    &MyLinRots_Param[1], 
    &MyChannels_Data[CHANNEL_9_DEST], 
    3, //number of channels
    MyLinRot1_DeltaCoeff,
    (TSL_tsignPosition_T *)TSL_POSOFF_3CH_ROT_M,
    TSL_SCTCOMP_3CH_ROT_M,
    0,
    MyLinRots_StateMachine, 
    &MyLinRots_Methods 
  }
};

//----------------
// Generic Objects
//----------------

// List (ROM)
CONST TSL_Object_T MyObjects[TSLPRM_TOTAL_OBJECTS] =
{
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[0] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[1] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[2] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[3] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[4] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[5] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[6] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[7] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[8] },
  { TSL_OBJ_TOUCHKEYB, (TSL_TouchKey_T *)&MyTKeysB[9] },
  { TSL_OBJ_LINEAR, (TSL_LinRot_T *)&MyLinRots[0] },
  { TSL_OBJ_ROTARY, (TSL_LinRot_T *)&MyLinRots[1] }
};

// Group (RAM)
TSL_ObjectGroup_T MyObjGroup =
{
  &MyObjects[0],        // First object
  TSLPRM_TOTAL_OBJECTS, // Number of objects
  0x00,                 // State mask reset value
  TSL_STATE_NOT_CHANGED // Current state
};

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
#if TSLPRM_TOTAL_TKEYS > 0  
  MyTKeys_StateMachine,   // Default state machine for TKeys
  &MyTKeys_Methods,       // Default methods for TKeys
#endif
#if TSLPRM_TOTAL_LNRTS > 0
  MyLinRots_StateMachine, // Default state machine for LinRots
  &MyLinRots_Methods      // Default methods for LinRots
#endif
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
  TSL_Status_enum_T status;
  
  if(!ConfigDone)
  {
    // Configure Bank
    TSL_acq_BankConfig(idx_bank);
 
    // Start Bank acquisition
    TSL_acq_BankStartAcq();
    
    // Set flag
    ConfigDone=1;
    
  }
  
  // Check Bank End of Acquisition
  if (TSL_acq_BankWaitEOC() == TSL_STATUS_OK)
  {
    // Get Bank Result
    STMSTUDIO_LOCK;
    TSL_acq_BankGetResult(idx_bank, 0, 0);
    STMSTUDIO_UNLOCK;
    ConfigDone=0;
    idx_bank++;
  }
  

  if (idx_bank == TSLPRM_TOTAL_BANKS)
  {
    idx_bank=0;
    
    // Process Objects
    TSL_obj_GroupProcess(&MyObjGroup);
  
    // DxS processing
    TSL_dxs_FirstObj(&MyObjGroup);

    // ECS every 100ms
    if (TSL_tim_CheckDelay_ms(100, &ECS_last_tick) == TSL_STATUS_OK)
    {
      TSL_ecs_Process(&MyObjGroup);
    }
    
    status = TSL_STATUS_OK; // All banks have been acquired and sensors processed
    
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
  // K4 thresholds modification
  //MyTKeysB[6].p_Param->ProxInTh = 3;
  //MyTKeysB[6].p_Param->ProxOutTh = 2; 
  MyTKeysB[6].p_Param->DetectInTh = 5;
  MyTKeysB[6].p_Param->DetectOutTh = 4;
  MyTKeysB[6].p_Param->CalibTh = 5;
  
  // K5 thresholds modification
  //MyTKeysB[7].p_Param->ProxInTh = 3;
  //MyTKeysB[7].p_Param->ProxOutTh = 2; 
  MyTKeysB[7].p_Param->DetectInTh = 5;
  MyTKeysB[7].p_Param->DetectOutTh = 4;
  MyTKeysB[7].p_Param->CalibTh = 5;
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
