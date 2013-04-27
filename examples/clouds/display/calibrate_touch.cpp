/**
 * \file calibrate_touch.cpp
 * \date 2013-4-27
 * \brief Aery32 Display Cloud Calibration Sequence. 
 * Run this example when using your Display Cloud the first time
 * or when your display needs calibration.
 *
 * Tap and hold on the marker for atleast one second. Release and the
 * next one appears, if not recycle the power and start again.
 *
 * \copyright
 * Copyright (c) 2013, Stemlux Systems Ltd. All rights reserved.
 * distributed under the New BSD license.
 * See accompanying file LICENSE.txt
 */

#include <aery32/all.h> // aery32 headers
#include "board.h" // platform settings
#include <display/all.h> // display cloud headers

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