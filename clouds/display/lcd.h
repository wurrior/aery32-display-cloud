/*   
 * File:	lcd.h
 * Author:	Markus Vuorio <markus.vuorio@stemlux.fi>
 * 
 * Copyright (c) 2013, Stemlux Systems Ltd.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Stemlux Systems nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL STEMLUX SYSTEMS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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