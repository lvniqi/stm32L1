/**
  ******************************************************************************
  * @file    STM32L152_Ex04_LPM_3TKeys_EVAL\inc\tsl_user.h
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
#include "lpm.h"

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
#define CHANNEL_0_SRC    ((uint32_t)(GR11))
#define CHANNEL_0_DEST   (0)
#define CHANNEL_0_SAMPLE    (TSL_GROUP11_IO4)
#define CHANNEL_0_CHANNEL   (TSL_GROUP11_IO1)

#define CHANNEL_1_SRC    ((uint32_t)(GR11))
#define CHANNEL_1_DEST   (1)
#define CHANNEL_1_SAMPLE    (TSL_GROUP11_IO4)
#define CHANNEL_1_CHANNEL   (TSL_GROUP11_IO2)

#define CHANNEL_2_SRC    ((uint32_t)(GR11))
#define CHANNEL_2_DEST   (2)
#define CHANNEL_2_SAMPLE    (TSL_GROUP11_IO4)
#define CHANNEL_2_CHANNEL   (TSL_GROUP11_IO3)

// Banks definition
#define BANK_0_NBCHANNELS    (1)
#define BANK_0_SHIELD_SAMPLE        (TSL_GROUP6_IO4)
#define BANK_0_SHIELD_CHANNEL       (TSL_GROUP6_IO3)

#define BANK_1_NBCHANNELS    (1)
#define BANK_1_SHIELD_SAMPLE        (TSL_GROUP6_IO4)
#define BANK_1_SHIELD_CHANNEL       (TSL_GROUP6_IO3)

#define BANK_2_NBCHANNELS    (1)
#define BANK_2_SHIELD_SAMPLE        (TSL_GROUP6_IO4)
#define BANK_2_SHIELD_CHANNEL       (TSL_GROUP6_IO3)


// User Parameters
extern CONST TSL_Bank_T MyBanks[];
extern CONST TSL_TouchKeyB_T MyTKeysB[];
extern CONST TSL_Object_T MyObjects[];
extern TSL_ObjectGroup_T MyObjGroup;

void MyTKeys_ErrorStateProcess(void);
void MyTKeys_OffStateProcess(void);

void TSL_user_Init(void);
TSL_Status_enum_T TSL_user_Action(void);

// STMStudio lock feature
void enterLock(void);
void exitLock(void);

#endif /* __TSL_USER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
