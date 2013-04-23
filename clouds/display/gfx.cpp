/*   
 * File:	gfx.cpp
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

#include <aery32/gpio.h>
#include "ascii.h"
#include "gfx.h"
#include "lcd.h"

BYTE ImageBuffer[GFX_BUFFER_SIZE];

void display::test_image()
{
	area_reset();
	aery::gpio_set_pin_low( CS );
	aery::gpio_set_pin_low( RS ); // write to register
	send_data_lcd( 0x22 );
	aery::gpio_set_pin_high( RS ); // write to data
	
	int i;
	for (i = 0; i < 12000; i++)
	{
		send_data_lcd( 0 );
	}
	for (i = 0; i < 12000; i++)
	{
	send_data_lcd( RED );
	}
	for (i = 0; i < 12000; i++)
	{
	send_data_lcd( YELLOW );
	}
	for (i = 0; i < 12000; i++)
	{
		send_data_lcd( GREEN );
	}
	for (i = 0; i < 12000; i++)
	{
		send_data_lcd( GREEN | BLUE );
	}
	for (i = 0; i < 12000; i++)
	{
		send_data_lcd( BLUE );
	}
	for (i = 0; i < 12000; i++)
	{
	send_data_lcd( RED | BLUE );
	}
	for (i = 0; i < 12000; i++)
	{
	send_data_lcd( WHITE );
	}			
	
	aery::gpio_set_pin_high( CS );
}

void display::set_pixel( unsigned int x, unsigned int y, unsigned int color )
{
	area_set(x,y,x,y);
	// set color
	set_reg_lcd( 0x22, color );
}

void display::fill_rectangle( unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color )
{
	area_set( x, y, x+width-1, y+height-1 );
		
	aery::gpio_set_pin_low( CS );
	aery::gpio_set_pin_low( RS ); // write to register
	send_data_lcd( 0x22 );
	aery::gpio_set_pin_high( RS ); // write to data

	unsigned int i, len = height * width;
	for (i = 0; i < len; i++)
	{
		send_data_lcd( color );
	}
	
	area_reset();
	
}

void display::draw_line( unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2, unsigned int color )
{
	    int	xofs,
	    yofs,
	    dx,		//deltas
	    dy,
	    dx2,	//scaled deltas
	    dy2,
	    ix,		//increase rate on the x axis
	    iy,		//increase rate on the y axis
	    err,	//the error term
	    i;		//looping variable

	    // difference between starting and ending points
	    dx = x2 - x1;
	    dy = y2 - y1;

	    // calculate direction of the vector and store in ix and iy
	    if (dx >= 0) ix = 1;
	    if (dx < 0)
	    {
		    ix = -1;
		    dx = -dx;
	    }

	    if (dy >= 0) iy = 1;
	    if (dy < 0)
	    {
		    iy = -1;
		    dy = -dy;
	    }

	    // scale deltas and store in dx2 and dy2
	    dx2 = dx << 1;
	    dy2 = dy << 1;
	    xofs=0;
	    yofs=0;
	    if (dx > dy) {	// dx is the major axis
	    // initialize the error term
	    err = dy2 - dx;
	    
	    for (i = 0; i <= dx; i++)
	    {
		    set_pixel(x1+xofs ,y1+yofs, color);
		    if (err >= 0)
		    {
			    err -= dx2;
			    yofs+=iy;
		    }
		    err += dy2;
		    xofs+=ix;
	    }
    }
	else
	{ 		// dy is the major axis
		// initialize the error term
		err = dx2 - dy;

		for (i = 0; i <= dy; i++)
		{
			set_pixel(x1+xofs, y1+yofs ,color);
			if (err >= 0) {
				err -= dy2;
				xofs+=ix;
			}
			err += dx2;
			yofs+=iy;
		}
    }
}

void display::show_image( unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *path )
{
	unsigned int bytesRead = 0, k = 0, offset = 0, pixel = 0;
	bool first = true, rgb1555 = false;
	FIL file;
	FRESULT rc;
	int bitsPerPixel = 0, rowSize = 0, rowDataSize = 0, bmpWidth, bmpHeight, padding, rowByteCounter = 0;
	// buffer needs to be divisible by 3 because pixels 3 byte long.
	unsigned int buffersize = GFX_BUFFER_SIZE-(GFX_BUFFER_SIZE%3);
	
	rc = f_open( &file, path, FA_READ );
	
	set_reg_lcd( 0x16, 0x90 );
	area_set( x, 400-(y+height), x+width-1, 400-y-1 );	
	
	aery::gpio_set_pin_low( CS );
	aery::gpio_set_pin_low( RS ); // write to register
	send_data_lcd( 0x22 );
	aery::gpio_set_pin_high( RS ); // write to data
	int j =0;
	while( !f_read( &file, ImageBuffer, buffersize, &bytesRead ) )
	{
		if( bytesRead == 0  ) break; //eof
		if(first) // get the bmp-header information from the first block
		{
			first = false; // deactivate
			
			// extract offset:
			offset = ImageBuffer[0xD];
			offset <<= 8;
			offset |= ImageBuffer[0xC];
			offset <<= 8;
			offset |= ImageBuffer[0xB];
			offset <<= 8;
			offset |= ImageBuffer[0xA];
			
			rgb1555 = ( offset < 0x40 );
			
			bmpWidth = ImageBuffer[0x12];
			bmpHeight = ImageBuffer[0x16];
			bitsPerPixel = ImageBuffer[0x1C];
			rowSize = ((bitsPerPixel * bmpWidth + 31)>>5)<<2;
			rowDataSize = ((bitsPerPixel>>3) * bmpWidth);
			padding = rowSize - rowDataSize;
		}
		else offset = 0; // for all other blocks
		// Transmit image data to the screen:
		enable_fast_transfer();
		switch( bitsPerPixel )
		{
			case 8:
				// not implemented yet
			break;
			case 16:
			for(k=offset; k < bytesRead-1; k+=2,rowByteCounter+=2, j++)
			{
				if( rowByteCounter == rowDataSize ) // end of row
				{
					j=0;
					rowByteCounter = 0;
					k += padding;
				}
				pixel = 0 | (ImageBuffer[ k+1 ])<<8 | (ImageBuffer[ k ]);
				if(rgb1555) pixel = ((pixel&0x7C00)<<1) |
									((pixel&0x3E0)<<1) |
									((pixel&0x1F));
				send_data_lcd_fast( pixel );
			}
			break;
			case 24:
			k = offset;
			while( k < bytesRead-2 )
			{
				pixel = (ImageBuffer[ k+2 ]<<16) | (ImageBuffer[ k+1 ]<<8) | (ImageBuffer[ k ]);
				send_data_lcd_fast(
				((pixel&0xF80000)>>8) |
				((pixel&0x00FC00)>>5) |
				((pixel&0x0000FF)>>3)
				);
				rowByteCounter += 3;
				if( rowByteCounter >= rowSize ) // new row
				{
					rowByteCounter = 0;
					k+=padding;
				}
				k +=3;
			}
			break;
		}
		disable_fast_transfer();	
	}
	
	rc = f_close(&file);
	area_reset();
	set_reg_lcd( 0x16, 0x10 );
}

void display::print_text_v( int x, int y, unsigned int color, char* txt )
{
	char *cp, col;
	uint8_t mask;
	int k, j, i, index;
	
	
	for( k=0, cp = txt; *cp != '\0'; cp++, k++ )
	{
		for( i = 0; i < 5; i++ ) // y
		{
			index = *cp-32;
			if( *cp > 0xA0 ) index -= 32;
			col = Ascii_1[ index ][i];
			mask = 1;
			for( j = 0; j < 8; j++ ) // x
			{
				if( mask & col ) set_pixel( x+8-j,y+i+(k*6),color );
				mask<<=1;
			}
			
		}
	}
}

void display::print_text_h( int x, int y, unsigned int color, char* txt )
{
	char *cp, col;
	uint8_t mask;
	int k, j, i, index;
	
	
	for( k=0, cp = txt; *cp != '\0'; cp++, k++ )
	{
		for( i = 0; i < 5; i++ ) // x
		{
			index = *cp-32;
			if( *cp > 0xA0 ) index -= 32;
			col = Ascii_1[ index ][i];
			mask = 1;
			for( j = 0; j < 8; j++ ) // y
			{
				if( mask & col ) set_pixel( x+i+(k*6), y+j,color );
				mask<<=1;
			}
			
		}		
	}
}

