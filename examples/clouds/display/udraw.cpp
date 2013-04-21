/**
 *	\file udraw.cpp
 *	\brief uDraw is a simple example on how to accept and handle touch reports.
 */

#include <aery32/all.h> // aery32 headers
#include "board.h" // platform settings
#include "clouds/display/display.h" // display headers

int main(void)
{
	display::touch_t current, last; // touch events

	board::init(); // initializes platform

	display::lcd_init(); // initializes lcd panel
	display::touch_init(); // initializes touch controller
	display::touch_enable(); // enables touch controller

	/* Empty the display by filling a 240x400px rectangle with 
	white(color code: 0xFFFF): */
	display::fill_rectangle(0,0,240,400,0xFFFF);

	for(;;) {
		// Wait for touch reports:
		display::touch_wait_for_data();
		// Get the received data:
		current = display::touch_get_data();

		// Compare pen states:
		if(current.penstate && last.penstate)
		{
			/* Both are 1, so pen is still down. 
			Draw a black line (color code: 0) 
			between last and current pen postion: */
			display::draw_line( current.x, current.y, last.x, last.y, 0 );
		}
		
		// store current event:	
		last = current;
		
	}

	return 0;
}