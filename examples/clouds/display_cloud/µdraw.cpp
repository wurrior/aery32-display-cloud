#include <aery32/all.h>
#include "board.h"
#include "clouds/display_cloud/display_cloud.h"

using namespace displayCloud;

int main(void)
{
 /*
 * 	µDraw
 *	This is a simple example on how to accept and handle touch reports.
 */
board::init();			//initialize Aery32
lcd_init();				//initialize display
touch_init();			//initialize touch controller
touch_enable();			//enable touch input

fill_rectangle(0,0,240,400,0xFFFF); //empty the display (fill the display from x,y to x,y, color FFFF "white")
touch_t touch;						//Setup some variables
int last_x = 0, last_y = 0;
uint8_t last_state = 0;

 for(;;) {

	touch_wait_for_data();															//Wait for touch reports
	touch = touch_get_data();														//Get the received data
		if(last_state && touch.penstate)											//Check if pen was lifted
			{
				displayCloud::draw_line( touch.x, touch.y, last_x, last_y, 0 );		//if not draw a line between two reports (from x,y to x,y color 0 "black")
			}

		last_x = touch.x;															//set the current report into variables
		last_y = touch.y;
		last_state = touch.penstate;
}
return 0;
}