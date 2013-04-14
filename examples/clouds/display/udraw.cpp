#include <aery32/all.h>
#include "board.h"

#include "clouds/display/display.h"

int main(void)
{
	/* Declare some variables */
	display::touch_t touch;
	int last_x = 0, last_y = 0;
	uint8_t last_state = 0;

	board::init();

	/* µDraw is a simple example on how to accept and handle touch reports */
	display::lcd_init();
	display::touch_init();
	display::touch_enable();

	/* Empty the display (fill the display from x,y to x,y, color FFFF "white") */
	display::fill_rectangle(0,0,240,400,0xFFFF);

	for(;;) {

		display::touch_wait_for_data();          //Wait for touch reports
		touch = display::touch_get_data();       //Get the received data
			if(last_state && touch.penstate) //Check if pen was lifted
				{
					display::draw_line( touch.x, touch.y, last_x, last_y, 0 ); //if not draw a line between two reports (from x,y to x,y color 0 "black")
				}

			last_x = touch.x; //set the current report into variables
			last_y = touch.y;
			last_state = touch.penstate;
	}

	return 0;
}