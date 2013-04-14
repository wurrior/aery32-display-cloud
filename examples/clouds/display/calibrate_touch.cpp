#include <aery32/all.h>
#include "board.h"

#include "clouds/display/display.h"

int main(void)
{
	board::init();

	/*
	 * Aery32 Display Cloud Calibration Sequence. Run this example when
	 * using your Display Cloud the first time or when your display needs
	 * calibrating.
	 *
	 * Tap and hold on the marker for atleast one second. Release and the
	 * next one appears, if not recycle the power and start again.
	 */
	display::lcd_init();        // Initialize display
	display::touch_init();      // Initialize touch controller
	display::touch_calibrate(); // Run the calibration sequence	
	display::test_image();      // Show that the calibration has ended
}