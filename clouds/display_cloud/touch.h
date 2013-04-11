/*   
 * File:	touch.h
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

#ifndef TOUCH_H_
#define TOUCH_H_

namespace displayCloud
{
	typedef struct {
		unsigned short x;
		unsigned short y;
		bool	penstate;
	} touch_t;
	/**
	 *	\brief Initialize the touch controller
	 *	This needs to be run before any other touch related 
	 *	functions
	 */
	void touch_init(void);
	/**
	 *	\brief Enables touch recognition
	 *	Controller needs to be calibrated before this works correctly.
	 */
	void touch_enable(void);
	/**
	 * \brief Disables the touch controller
	 */
	void touch_disable(void);
	/**
	 *	\brief Calibrate the touch controller.
	 *	Displays the calibration screen.
	 *	Tap on the crosses in the corned to calibrate.
	 *	Calibration data is stored in the controller's EEPROM
	 */
	void touch_calibrate(void);
	/**
	 *	\brief Send data to the touch controller
	 *	\param arr[] Data array
	 *	\param size  Data array size in bytes
	 */
	void touch_send_data( unsigned char arr[], int size );
	/**
	 *	\brief Set value of a touch controller register
	 *	\param reg Register number
	 *	\param val Register value
	 */
	void touch_set_reg( int reg=0, int val=0 );
	/**
	 *	\brief waits until touch controller registers touch 
	 */
	void touch_wait_for_data(void);
	/**
	 *	\brief Reads values from controller
	 *	Run touch_wait_for_data() before this function;
	 */
	touch_t touch_get_data(void);
	
}

#endif /* TOUCH_H_ */