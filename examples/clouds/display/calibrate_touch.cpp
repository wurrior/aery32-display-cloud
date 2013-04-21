/**
 * \file calibrate_touch.cpp
 * \brief Aery32 Display Cloud Calibration Sequence. 
 * Run this example when using your Display Cloud the first time
 * or when your display needs calibration.
 *
 * Tap and hold on the marker for atleast one second. Release and the
 * next one appears, if not recycle the power and start again.
 */

#include <aery32/all.h> // aery32 headers
#include "board.h" // platform settings
#include "clouds/display/display.h" // display cloud headers

int main(void)
{
	// Initialize aery32:
	board::init();				
	// Initialize lcd panel:
	display::lcd_init();
	// Initialize touch controller:
	display::touch_init();
	// Run the calibration sequence:
	display::touch_calibrate();
	// Show test screen to mark the end of calibration:
	display::test_image();      
}