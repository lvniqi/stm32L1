/**
  ******************************************************************************
  * @file    STM32L152_Ex07_10TKeys_Lin_Rot_TS_EVAL\inc\tsl_user.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSL_USER_H
#define __TSL_USER_H

#include "tsl.h"

// STMStudio software usage (0=No, 1=Yes)
// Warning: The low-power mode must be disabled when STMStudio is used.
#define USE_STMSTUDIO (1)

#if USE_STMSTUDIO > 0
#define STMSTUDIO_LOCK {enterLock();}
#define STMSTUDIO_UNLOCK {exitLock();}
#else
#define STMSTUDIO_LOCK
#define STMSTUDIO_UNLOCK
#endif

//==============================================================================
// IOs definition
//==============================================================================

// Channel IOs definition
//For Bank 1
//K2
#define CHANNEL_0_SRC    ((uint32_t)(GR8))
#define CHANNEL_0_DEST   (0)
#define CHANNEL_0_SAMPLE    (TSL_GROUP8_IO1)
#define CHANNEL_0_CHANNEL   (TSL_GROUP8_IO2)

//K3
#define CHANNEL_1_SRC    ((uint32_t)(GR1))
#define CHANNEL_1_DEST   (1)
#define CHANNEL_1_SAMPLE    (TSL_GROUP1_IO1)
#define CHANNEL_1_CHANNEL   (TSL_GROUP1_IO2)

//K9
#define CHANNEL_2_SRC    ((uint32_t)(GR9))
#define CHANNEL_2_DEST   (2)
#define CHANNEL_2_SAMPLE    (TSL_GROUP9_IO1)
#define CHANNEL_2_CHANNEL   (TSL_GROUP9_IO2)

//K10
#define CHANNEL_3_SRC    ((uint32_t)(GR3))
#define CHANNEL_3_DEST   (3)
#define CHANNEL_3_SAMPLE    (TSL_GROUP3_IO1)
#define CHANNEL_3_CHANNEL   (TSL_GROUP3_IO2)

//Linear A
#define CHANNEL_4_SRC    ((uint32_t)(GR4))
#define CHANNEL_4_DEST   (4)
#define CHANNEL_4_SAMPLE    (TSL_GROUP4_IO1)
#define CHANNEL_4_CHANNEL   (TSL_GROUP4_IO2)

//Linear B
#define CHANNEL_5_SRC    ((uint32_t)(GR7))
#define CHANNEL_5_DEST   (5)
#define CHANNEL_5_SAMPLE    (TSL_GROUP7_IO1)
#define CHANNEL_5_CHANNEL   (TSL_GROUP7_IO2)

//Linear C
#define CHANNEL_6_SRC    ((uint32_t)(GR10))
#define CHANNEL_6_DEST   (6)
#define CHANNEL_6_SAMPLE    (TSL_GROUP10_IO1)
#define CHANNEL_6_CHANNEL   (TSL_GROUP10_IO2)

//For Bank 2
//K1
#define CHANNEL_7_SRC    ((uint32_t)(GR1))
#define CHANNEL_7_DEST   (7)
#define CHANNEL_7_SAMPLE    (TSL_GROUP1_IO1)
#define CHANNEL_7_CHANNEL   (TSL_GROUP1_IO3)

//K6
#define CHANNEL_8_SRC    ((uint32_t)(GR8))
#define CHANNEL_8_DEST   (8)
#define CHANNEL_8_SAMPLE    (TSL_GROUP8_IO1)
#define CHANNEL_8_CHANNEL   (TSL_GROUP8_IO3)

//Rotary B
#define CHANNEL_9_SRC    ((uint32_t)(GR10))
#define CHANNEL_9_DEST   (9)
#define CHANNEL_9_SAMPLE    (TSL_GROUP10_IO1)
#define CHANNEL_9_CHANNEL   (TSL_GROUP10_IO3)

//Rotary C
#define CHANNEL_10_SRC    ((uint32_t)(GR4))
#define CHANNEL_10_DEST   (10)
#define CHANNEL_10_SAMPLE    (TSL_GROUP4_IO1)
#define CHANNEL_10_CHANNEL   (TSL_GROUP4_IO3)

//Rotary A
#define CHANNEL_11_SRC    ((uint32_t)(GR7))
#define CHANNEL_11_DEST   (11)
#define CHANNEL_11_SAMPLE    (TSL_GROUP7_IO1)
#define CHANNEL_11_CHANNEL   (TSL_GROUP7_IO3)

//For Bank 3
//K4
#define CHANNEL_12_SRC    ((uint32_t)(GR1))
#define CHANNEL_12_DEST   (12)
#define CHANNEL_12_SAMPLE    (TSL_GROUP1_IO1)
#define CHANNEL_12_CHANNEL   (TSL_GROUP1_IO4)

//K5
#define CHANNEL_13_SRC    ((uint32_t)(GR8))
#define CHANNEL_13_DEST   (13)
#define CHANNEL_13_SAMPLE    (TSL_GROUP8_IO1)
#define CHANNEL_13_CHANNEL   (TSL_GROUP8_IO4)

//K7
#define CHANNEL_14_SRC    ((uint32_t)(GR10))
#define CHANNEL_14_DEST   (14)
#define CHANNEL_14_SAMPLE    (TSL_GROUP10_IO1)
#define CHANNEL_14_CHANNEL   (TSL_GROUP10_IO4)

//K8
#define CHANNEL_15_SRC    ((uint32_t)(GR7))
#define CHANNEL_15_DEST   (15)
#define CHANNEL_15_SAMPLE    (TSL_GROUP7_IO1)
#define CHANNEL_15_CHANNEL   (TSL_GROUP7_IO4)

// Banks definition
#define BANK_0_NBCHANNELS    (7)
#define BANK_0_SHIELD_SAMPLE        (0)
#define BANK_0_SHIELD_CHANNEL       (0)

#define BANK_1_NBCHANNELS    (5)
#define BANK_1_SHIELD_SAMPLE        (0)
#define BANK_1_SHIELD_CHANNEL       (0)

#define BANK_2_NBCHANNELS    (4)
#define BANK_2_SHIELD_SAMPLE        (0)
#define BANK_2_SHIELD_CHANNEL       (0)



// [IT disabling for IO protection] definition (value is '0' for no and '1' for yes)
#define PROTECT_IO_ACCESS (1)

// User Parameters
extern CONST TSL_Bank_T MyBanks[];
extern CONST TSL_TouchKeyB_T MyTKeysB[];
extern CONST TSL_LinRot_T MyLinRots[];
extern CONST TSL_Object_T MyObjects[];
extern TSL_ObjectGroup_T MyObjGroup;

void MyTKeys_ErrorStateProcess(void);
void MyTKeys_OffStateProcess(void);

void MyLinRots_ErrorStateProcess(void);
void MyLinRots_OffStateProcess(void);

void TSL_user_Init(void);
TSL_Status_enum_T TSL_user_Action(void);

// STMStudio lock feature
void enterLock(void);
void exitLock(void);

#endif /* __TSL_USER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
