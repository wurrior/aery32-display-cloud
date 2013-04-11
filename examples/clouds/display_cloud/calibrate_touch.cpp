#include <aery32/all.h>
#include "board.h"
#include "clouds/display_cloud/display_cloud.h"

using namespace displayCloud;

int main(void)
{
 /*
 * Aery32 Display Cloud Calibration Sequence
 * Run this example when using your Display Cloud the first time
 * or when your display needs calibrating
 *
 * Tap and hold on the marker for atleast one second. Release and the next one appears, if not recycle the power and start again. 
 */
board::init();			//initialize Aery32
lcd_init();				//initialize display
touch_init();			//initialize touch controller
touch_calibrate();		//Run the calibration sequence	
test_image();			//Show that the calibration has ended
}