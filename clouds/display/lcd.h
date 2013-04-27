/**   
 * \file lcd.h
 * \date 2013-4-27
 * \brief lcd
 *
 * \copyright
 * Copyright (c) 2013, Stemlux Systems Ltd. All rights reserved.
 * distributed under the New BSD license.
 * See accompanying file LICENSE.txt
 * 
 */

#ifndef LCD_H_
#define LCD_H_

#include "fatfs/ff.h"

// PINS:
#define RD				AVR32_PIN_PB19 // AVR32_PIN_PB20
#define WR				AVR32_PIN_PB20 // AVR32_PIN_PB19
#define RS				AVR32_PIN_PB21
#define CS				AVR32_PIN_PB22
#define RST				AVR32_PIN_PB23 // AVR32_PIN_PB18
#define DISPLAY_LED			AVR32_PIN_PB24
#define FMARK				AVR32_PIN_PB25
#define DATAMASK			0x0003FFFF

#define SDO				AVR32_PIN_PB27

// MACROS:
/**
 *	\brief Send data the display through th GPIO module
 */
#define send_data_lcd( dat ) AVR32_GPIO.port[1].ovrc = DATAMASK;\
		AVR32_GPIO.port[1].ovrs = DATAMASK & dat;\
		AVR32_GPIO.port[1].ovrc = 0x100000;\
		AVR32_GPIO.port[1].ovrs = 0x100000;

/**
 *	\brief Send data to the display using local bus
 *	Before this fucntion can be used enable_fast_transfer() 
 * 	must be called to activate local bus mode.
 */
#define send_data_lcd_fast( dat ) AVR32_GPIO_LOCAL.port[1].ovrc = DATAMASK;\
		AVR32_GPIO_LOCAL.port[1].ovrs = DATAMASK & dat;\
		AVR32_GPIO_LOCAL.port[1].ovrc = 0x100000;\
		AVR32_GPIO_LOCAL.port[1].ovrs = 0x100000;
/**
*	\brief enable local bus mode and fast transfer
*/
#define enable_fast_transfer() 	__builtin_mtsr(AVR32_CPUCR, __builtin_mfsr(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);
/**
 *	\brief disables local bus mode
 */
#define disable_fast_transfer() __builtin_mtsr(AVR32_CPUCR, __builtin_mfsr(AVR32_CPUCR) & ~AVR32_CPUCR_LOCEN_MASK);


namespace display {
/**
 *	\brief Initialize the LCD panel
 *	This must be called before any other display related functions
 */
void lcd_init( void );
/**
 *	\brief Set value of a display driver register
 */
void set_reg_lcd( unsigned char add, unsigned short val );
/**
 * \brief Read the value of a display driver register
 */
int read_reg_lcd( unsigned short reg );
/**
 *	\brief Set RAM access area
 */
void area_set( unsigned int xb, unsigned int yb, unsigned int xe, unsigned int ye );
/**
 *	\brief Set RAM access area to default size (full screen)
 */
void area_reset();

} /* end of namespace display */

#define DISPLAY_32

#endif /* LCD_H_ */