/*   
 * File:	touch.cpp
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

#include <aery32/all.h>
#include "touch.h"
#include "lcd.h"
#include "gfx.h"

namespace displayCloud // private functions
{
	unsigned char buffer[5];
	/**
	 *	\brief Wait for user to touch the display during calibration
	 */
	void checkpoint();	
}

void displayCloud::checkpoint()
{
	unsigned char arr1[4];
	do{
		arr1[0] = 0;
		arr1[2] = 0;
		arr1[1] = 0;
		arr1[3] = 0;
		aery::delay_ms(20);
		aery::twi_read_nbytes( arr1, 4 );
	} while( (arr1[0] != 0x55) || arr1[2] || (arr1[3] != 0x14) );
}

void displayCloud::touch_init()
{
	aery::gpio_init_pins( aery::porta, (1<<AVR32_PIN_PA29) | (1<<AVR32_PIN_PA30), GPIO_FUNCTION_A | GPIO_OPENDRAIN);
	aery::gpio_init_pin( SDO, GPIO_INPUT );
	aery::twi_init_master();
	aery::delay_ms(100);
}

void displayCloud::touch_calibrate()
{
	touch_disable();
	
	color16_t cross_color, bg_color;
	bg_color.u = 0xFFFF;
	
	fill_rectangle( 0, 0, 240, 400, 0 );
	print_text_v(115, 120, bg_color.u, "Touch And Release Target");
	
	aery::twi_select_slave( 0x4D );
	
	uint8_t arr0[] = {0x55,0x05,0x21,0x00,0x2E,0x01,0x19};
	touch_send_data(arr0,7);

	unsigned char arr[4];

	do {
		arr[0] = 0x55;
		arr[1] = 0x02;
		arr[2] = 0x14;
		arr[3] = 0x04;

		aery::delay_ms(1000);
		touch_send_data(arr,4);
	}
	while( (arr[2]!=0) || (arr[3] != 0x14) );

	// upper left corner
	draw_line(20,50,40,50,bg_color.u);
	draw_line(30,40,30,60,bg_color.u);
	checkpoint(); // first point

	// upper right corner
	draw_line(200,50,220,50,bg_color.u);
	draw_line(210,40,210,60,bg_color.u);
	checkpoint(); // 2nd point

	// lower left corner
	draw_line(200,350,220,350,bg_color.u);
	draw_line(210,340,210,360,bg_color.u);
	checkpoint(); //3rd point

	// lower right corner
	draw_line(20,350,40,350,bg_color.u);
	draw_line(30,340,30,360,bg_color.u);
	checkpoint(); //4th point

	checkpoint();// eeprom ready
}

void displayCloud::touch_enable()
{
	aery::twi_select_slave( 0x4D );
	unsigned char arr[5] = {0x55,0x01,0x12};
	
	touch_send_data( arr, 3 );
	
	if (arr[0x2] == 4 )
	{
		aery::delay_ms(50);
		touch_enable();
	}
}

void displayCloud::touch_send_data( uint8_t arr[], int size )
{
	aery::twi_select_slave( 0x4D );
	aery::twi_write_nbytes( arr, size );
	while( aery::gpio_read_pin(SDO) == 0 ) asm("nop");
	aery::delay_ms(50);
	aery::twi_read_nbytes( arr, 4 );

}

void displayCloud::touch_disable()
{
	aery::twi_select_slave( 0x4D );
	unsigned char arr[5] = {0x55,0x01,0x13};
	volatile int bytes =0;
	bytes = aery::twi_write_nbytes( arr, 3 );
	while( aery::gpio_read_pin(SDO) == 0 ) asm("nop");
	bytes = aery::twi_read_nbytes( arr, 4 );

	aery::delay_ms(50); // wait 50 ms before ANY COMMAND
	if (arr[0x2] != 0 )
	{		
		touch_disable();
	}
}

void displayCloud::touch_set_reg( int reg, int val )
{
	
	int addressoffet = 0x20; // TODO: read from chip
	uint8_t arr[8];
	
	do {
		arr[0] = 0x55;
		arr[1] = 0x05;
		arr[2] = 0x21;
		arr[3] = 0x00;
		arr[4] = reg + addressoffet;
		arr[5] = 0x01;
		arr[6] = val;		
		touch_send_data( arr, 7 );
	} while( (arr[0] != 0x55) || (arr[1] != 0x02) || (arr[2] != 0) || (arr[3] != 0x21) );

	aery::delay_ms(50);
}

 void displayCloud::touch_wait_for_data(void)
 {
	while( !aery::gpio_read_pin(SDO) )  asm("nop");
 }
 
 displayCloud::touch_t displayCloud::touch_get_data()
 {
	touch_t touch;
	aery::twi_read_nbytes( buffer, 5 );
	touch.x = ((buffer[1]>>2) | (buffer[2]<<5))/4.267;
	touch.y = ((buffer[3]>>2) | (buffer[4]<<5))/2.56;
	touch.penstate = (buffer[0] & 1);
	return touch;
 }