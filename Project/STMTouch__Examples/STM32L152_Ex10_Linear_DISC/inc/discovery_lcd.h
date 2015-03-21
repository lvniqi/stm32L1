/**
  ******************************************************************************
  * @file    STM32L152_Ex10_Linear_DISC\inc\discovery_lcd.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    24-February-2014
  * @brief   This file contains all the functions prototypes for the glass LCD
  *          firmware driver.
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
#ifndef __stm32l_discovery_lcd
#define __stm32l_discovery_lcd

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/* Define for scrolling sentences*/
#define SCROLL_SPEED   400//20
#define SCROLL_SPEED_L  40
#define SCROLL_NUM     1

/* Define for character '.' */
#define  POINT_OFF 0
#define  POINT_ON  1

/* Define for caracter ":" */
#define  COLUMN_OFF 0
#define  COLUMN_ON  1

#define DOT 0x8000 /* for add decimal point in string */
#define DOUBLE_DOT 0x4000 /* for add decimal point in string */

/* Macros used for set/reset bar LCD bar */
#define BAR0_ON  t_bar[1] |= 8
#define BAR0_OFF t_bar[1] &= ~8
#define BAR1_ON  t_bar[0] |= 8
#define BAR1_OFF t_bar[0] &= ~8
#define BAR2_ON  t_bar[1] |= 2
#define BAR2_OFF t_bar[1] &= ~2
#define BAR3_ON t_bar[0]  |= 2
#define BAR3_OFF t_bar[0] &= ~2

/* code for 'µ' character */
#define C_UMAP 0x6084

/* code for 'm' character */
#define C_mMap 0xb210

/* code for 'n' character */
#define C_nMap 0x2210

/* constant code for '*' character */
#define star 0xA0DD

/* constant code for '-' character */
#define C_minus 0xA000

/* constant code for ASCII 204 character */
#define ASCII204 0xB888

void LCD_bar(void);
void LCD_GLASS_Init(void);
void LCD_GLASS_WriteChar(uint8_t* ch, uint8_t point, uint8_t column, uint8_t position);
void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_DisplayStrDeci(uint16_t* ptr);
void LCD_GLASS_ClearChar(uint8_t position);
void LCD_GLASS_Clear(void);
void LCD_GLASS_ScrollSentence(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);
void LCD_GLASS_WriteTime(char a, uint8_t posi, uint8_t column);

#endif /* __stm32l_discovery_lcd */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
