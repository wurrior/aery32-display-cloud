/**
 * \file brightness_control.cpp
 * \date 2013-4-27
 * \brief A simple example on how to adjust the backlight brightness.
 *
 * \copyright
 * Copyright (c) 2013, Stemlux Systems Ltd. All rights reserved.
 * distributed under the New BSD license.
 * See accompanying file LICENSE.txt
 */

#include <aery32/all.h> // aery32 headers
#include "board.h" // platform settings
#include <display/all.h> // display headers

int main(void)
{
	board::init(); // initializes platform

	display::lcd_init(); // initializes lcd panel

	/* Empty the display by filling a 400x240px rectangle with 
	white(color code: 0xFFFF): */
	display::fill_rectangle(0,0,400,240,0xFFFF);
	
	for(;;) 
	{	
		/*
		You can put your desired brightness value, from 0 to 31,
		inside the set_brightness funtion.
		0 is totally off an 31 is fully on.
		Smaller numbers than 0 are interpreted as 0
		and larger numbers than 31 are interpreted as 31 respectively.
		*/
	
		for(int i=0; i<=31; i++) //increase brightness gradually from 0 to 31
		{								
			display::set_brightness(i);		
			aery::delay_ms(100); 			
		}
		
		for(int i=31; i>=0; i--) //decrese brightness gradually from 31 to 0
		{
			display::set_brightness(i);	
			aery::delay_ms(100); 
		}
		
	}

	return 0;
}