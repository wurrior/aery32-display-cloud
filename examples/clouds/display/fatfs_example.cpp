/** \file fatfs_example.cpp
 *	\brief Read and write text to sd card through FatFs
 *	This program creates a file writes "hello!" to it and reads it again to
 *	show the file content on the screen. 
 *	If the program fails "fail!" will be displayed instead.
 *	For more information about the FatFs library goto: 
 *	http://elm-chan.org/fsw/ff/00index_e.html
 */

#include <aery32/all.h>
#include "board.h"
#include <display/all.h>

using namespace aery;

int main(void)
{
	/*
	 * The default board initializer defines all pins as input and
	 * sets the CPU clock speed to 66 MHz.
	 */
	board::init();
	display::lcd_init();
	
	unsigned int bc = 0; // byte counter, required by f_read() and f_write()
	
	/* Declare a file system object: */
	FATFS fs;
	
	/* Declare a file object: */
	FIL text_file;
	
	/* Create buffers to store strings: */
	char input_buffer[8] = "hello!";
	char output_buffer[8] = "fail!";	
	
	/* Mount sdcard to fs: */
	f_mount( CARD_DRIVE, &fs ); // CARD_DRIVE is declared in sdcard.h
	
	/* Create a file by opening it */
	f_open( &text_file, "hello.txt", FA_CREATE_ALWAYS | FA_WRITE );
	
	/* write to file: */
	f_write( &text_file, input_buffer, 6, &bc );
	
	/* Close the file and open it again for reading: */
	f_close( &text_file );
	f_open( &text_file, "hello.txt", FA_OPEN_EXISTING | FA_READ );
	
	/* Read from file and print it on screen: */
	f_read( &text_file, output_buffer, 8, &bc );
	
	display::fill_rectangle(0,0,240,400,BLACK); // background
	display::print_text(0,0,WHITE,4,output_buffer); // print
	
	/* Close the file: */
	f_close( &text_file );
	
	for(;;) {
		
	}

	return 0;
}