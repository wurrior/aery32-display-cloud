/**
 *	\file brightness_control.cpp
 *	\brief brightness control is a simple example on how to adjust the backlight brightness.
 */

#include <aery32/all.h> // aery32 headers
#include "board.h" // platform settings
#include "clouds/display/display.h" // display headers

int main(void)
{
	board::init(); // initializes platform

	display::lcd_init(); // initializes lcd panel

	/* Empty the display by filling a 240x400px rectangle with 
	white(color code: 0xFFFF): */
	display::fill_rectangle(0,0,240,400,0xFFFF);
	
	for(;;) 
	{	
		for(int i=0; i<32; i++)			
		{								
		display::set_brightness(i);		//You can put your desired value inside the set_brightness funtion, as long the value is between 0 and 31.
		aery::delay_ms(100); 			//0 is totally off an 31 is fully on.
		}
		
		for(int i=31; i>0; i--)			
		{
		display::set_brightness(i);	
		aery::delay_ms(100); 
		}
		
	}

	return 0;
}