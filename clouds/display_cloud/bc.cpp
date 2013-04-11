/*   
 * File:	bc.cpp
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

#include "display_cloud.h"
#include "../board.h"
#include <aery32/all.h>

namespace displayCloud {
	// store the current value for brightness:
	char current_brightness = 32; 
}

void displayCloud::decrease_brightness( int steps )
{
	for( int i = 0; i< steps; i++ )
	{
		aery::gpio_set_pin_low( DISPLAY_LED );
		aery::delay_us(1);
		aery::gpio_set_pin_high( DISPLAY_LED );
	}
}

char displayCloud::get_brightness_level( void )
{
	return current_brightness;
}

void displayCloud::increase_brightness( int steps )
{
	/* the controller only supports decreasing */
	decrease_brightness( 32 - steps );
}

void displayCloud::set_brightness( char val )
{
	if( val > 32 ) val = 32;
	if( val < 0 ) val = 0;
	if( val < current_brightness )
		displayCloud::decrease_brightness( current_brightness - val );
	else if ( val > current_brightness )
		displayCloud::decrease_brightness( 32 - (val - current_brightness) );
}