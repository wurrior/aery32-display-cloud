/*   
 * File:	gfx.h
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

#ifndef GFX_H_
#define GFX_H_

// Define buffer size used by show_image():
#define GFX_BUFFER_SIZE 12000 // in Bytes

// Uncummend following line, you dont use FATFS:
// #define NO_FATFS

namespace displayCloud
{
	typedef union // 16bit color
	{
		unsigned int u;
		struct
		{
			unsigned int :  16;
			unsigned int r : 5;
			unsigned int g : 6;
			unsigned int b : 5;	
		} rgb;
	} color16_t;
	
	/**
	 *	\brief Display colored stripes on the screen
	 */
	void test_image( void );
	/**
	 *	\brief Changes the color of the specified pixel
	 */
	void set_pixel( unsigned int x,
					unsigned int y,
					unsigned int color
					);
	/**
	 *	\brief Prints text in vertical direction
	 */
	void print_text_v(	int x,
						int y,
						unsigned int color,
						char* txt 
					);
	/**
	 *	\brief Prints text in horizontal direction
	 */
	void print_text_h(	int x,
						int y,
						unsigned int color,
						char* txt
					);
	/**
	 *	\brief Draw a line on the screen
	 *   line is drawn from (x1,y1) to (x2,y2)
	 */
	void draw_line(	unsigned int x1, // start point x
					unsigned int y1, // start point y
					unsigned int x2, // end point x
					unsigned int y2, // end point y
					unsigned int color
					);
	/**
	 *	\brief Draw and fill a rectangle with the specified color
	 */
	void fill_rectangle(	unsigned int x,
							unsigned int y,
							unsigned int width,
							unsigned int height,
							unsigned int color
						);
	#ifndef NO_FATFS
	/**
	 *	\brief Displays an Windows BMP file on the screen.
	 *	Supported formats:	16bit( x-5-5-5 and 5-6-5 )
							24bit
	 *	Currently no scaling is supported, so height and width have to match the
	 *	files height and with. 
	 */
	void show_image(	unsigned int x, 
						unsigned int y, 
						unsigned int width, 
						unsigned int height, 
						const char *path
					);
	#endif
}
#endif /* GFX_H_ */