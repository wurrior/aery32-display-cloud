/*   
 * File:	lcd.cpp
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
 
#include "common.h"
#include "display_cloud.h"

extern bool buttonPressed; // indicates a tap on the screen

#ifdef DISPLAY_32

/* 
	The display driver chip has a bug that does not allow updating the 
	memory access registers with the same values already in them.
	Therefor the current values are stored and checked on each call of
	the area_set() method.
*/
unsigned int write_area_x_begin = 0,
write_area_y_begin = 0,
write_area_x_end = 240,
write_area_y_end = 400;


int displayCloud::read_reg_lcd( unsigned short reg )
{
	unsigned short temp;
	aery::gpio_set_pin_low( CS );
	aery::gpio_set_pin_low( RS ); // write to register
	send_data_lcd( reg );
	aery::gpio_set_pin_high( RS ); //done
	
	aery::gpio_init_pins(aery::portb, DATAMASK, GPIO_INPUT);
	aery::gpio_set_pin_low( RD ); // read register
	aery::delay_ms(1);
	temp = AVR32_GPIO.port[ 1 ].pvr & DATAMASK;
	aery::gpio_set_pin_high( RD ); // done
	aery::gpio_set_pin_high( CS );
	
	aery::gpio_init_pins(aery::portb, DATAMASK, GPIO_OUTPUT);
	
	return temp;
}

void displayCloud::set_reg_lcd( unsigned char add, unsigned short val )
{	
	aery::gpio_set_pin_low( CS );
	aery::gpio_set_pin_low( RS ); // write to register
	send_data_lcd( add );
	aery::gpio_set_pin_high( RS ); // write to data
	send_data_lcd( val );
	aery::gpio_set_pin_high( CS );
}

void displayCloud::lcd_init( void )
{	
	aery::delay_ms(10);
	aery::gpio_init_pins( aery::portb, 0xffffffff, GPIO_OUTPUT );
	aery::gpio_init_pin( FMARK, GPIO_INPUT );
	aery::gpio_set_pin_high( RD ); // set, nothing to be read
	aery::gpio_set_pin_high( RS ); // set
	aery::gpio_set_pin_high( CS ); // set
	aery::gpio_set_pin_high( RST );
	aery::delay_ms(10);
	
	//PANEL INTERFACE SETTINGS
	set_reg_lcd( 0x17, 0x01 ); // clocks per line to min
	set_reg_lcd( 0x2E, 0x00 );
	set_reg_lcd( 0x2F, 0x00 );
	set_reg_lcd( 0x30, 0x00 );
	set_reg_lcd( 0x31, 0x00 );
	set_reg_lcd( 0x32, 0x00 );
	
	//set_reg_lcd( 0x23, 0x01 ); // Fmark (TE) On

	// POWER SETTINGS:	
	set_reg_lcd( 0x1A, 0x57 ); // pc2 bt-setting no 1
	set_reg_lcd( 0x1B, 0x01 ); // pc3 1 x VCILVL
	set_reg_lcd( 0x1C, 0x1E ); // pc4 PON? VREGOUT1 x 1.32
	set_reg_lcd( 0x1D, 0x0F ); // pc5 VCIR x 1.6,
	set_reg_lcd( 0x8A, 0x07 ); // Power driving reg
	set_reg_lcd( 0x85, 0x07 );
	//set_reg( 0x58, 0x80 );
	
	set_reg_lcd( 0x1F, 0x75 ); // 75 vcom
	set_reg_lcd( 0x19, 0x12 ); // PC1 PSON and PON
	aery::delay_ms(50);
	set_reg_lcd( 0x1E, 0x36 ); // pc6
	aery::delay_ms(150);

	//DISPLAY SETTINGS
	set_reg_lcd( 0x55, 0x07 ); //RGB and Frame inversion (Panel Control register)
	set_reg_lcd( 0x3A, 0xC0 );
	set_reg_lcd( 0x3C, 0xFF ); // nsap
	set_reg_lcd( 0x3D, 0xFF ); // isap
	set_reg_lcd( 0x24, 0x3C ); // display enable
	set_reg_lcd( 0x91, 0x01 );
	
	//RAM SETTINGS:
	set_reg_lcd(0x16, 0x80);
	set_reg_lcd(0x2, 0);
	set_reg_lcd(0x3, 0);
	
	set_reg_lcd(0x4,0);
	set_reg_lcd(0x5,0xF0);
	
	set_reg_lcd(0x6, 0);
	set_reg_lcd(0x7, 0);	
	
	set_reg_lcd( 0x8, 0x01 ); // ram vertical end 399
	set_reg_lcd( 0x9, 0x90 );
	
	// LED on
	aery::gpio_set_pin_high( DISPLAY_LED );
	
	// Show test image to verify settings:
	test_image();
}

void displayCloud::area_reset()
{
	area_set( 0, 0, 239, 399 );
}

void displayCloud::area_set( unsigned int xb, unsigned int yb, unsigned int xe, unsigned int ye )
{
		// move ram pointer if needed
		if( write_area_x_begin != xb )
		{
			set_reg_lcd( 0x02, 0 );
			set_reg_lcd( 0x03, xb );
			write_area_x_begin = xb;
		}
		
		if ( write_area_y_begin != yb )
		{
			set_reg_lcd( 0x06, (yb>>8) );
			set_reg_lcd( 0x07, yb );
			write_area_y_begin = yb;
		}
		
		if( write_area_x_end != xe )
		{
			set_reg_lcd( 0x04, 0 );
			set_reg_lcd( 0x05, xe);
			write_area_x_end = xe;
		}
		
		if( write_area_y_end != ye )
		{
			set_reg_lcd( 0x08, (ye)>>8 );
			set_reg_lcd( 0x09, (ye) );
			write_area_y_end = ye;
		}
}



#endif