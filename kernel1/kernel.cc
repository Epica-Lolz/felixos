/*
   Felix OS meme kernel

   cursed incomplete c++ kernel stuff
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

//if the compile target is "linux", throw an error
#if defined(__linux__)
#error "You are not using a cross compiler"
#endif

//if the compile target is not i386, throw an error
#if !defined(__i386__)
#error "Not using a ix86-elf compiler"
#endif

//terminal max width and height
#define TERM_WIDTH	80
#define TERM_HEIGHT	25

//list of colors
enum colors{
	black		= 0x0,
	blue		= 0x1,
	green		= 0x2,
	cyan 		= 0x3,
	red 		= 0x4,
	magenta 	= 0x5,
	brown 		= 0x6,
	light_grey 	= 0x7,
	dark_grey 	= 0x8,
	light_blue 	= 0x9,
	light_green 	= 0xA,
	light_cyan 	= 0xB,
	light_red 	= 0xC,
	light_magenta 	= 0xD,
	light_brown 	= 0xE,
	white 		= 0xF
};

//values for current positions for the terminal row and column
size_t		term_row;
size_t		term_col;

//value for default terminal color
uint8_t		global_term_color;

//pointer to frame buffer
uint16_t* 	frame_buffer;

//current terminal position
size_t		index;

/*
when writing a character to the frame buffer, the character with foreground and background colors are stored in 16-bit cells, where the foreground color is
stored in the highest 4-bits, the background color is stored in bits 11-8, and the character itself is stored in bits 7-0, as shown here:

Bit:		|15 14 13 12|11 10 9 8|7 6 5 4 3 2 1 0|
Content:	|bg         |fg       |ASCII character|

for instance, the character 'S' with foreground color dark grey and background color black would be stored as 0x0853, where '0'
is the color 'black', '8' is the color 'dark grey', and '53' is the ASCII character 'S'.
*/

//return an 8 bit value with the foreground at the lowest 4-bits, and the background at the highest 8-bits.
static inline uint8_t vga_color(enum colors fg, enum colors bg){
	return ((uint8_t)fg | ((uint8_t)bg << 4));
}


//return a 16-bit value with the character at the lowest 8-bits, and the color at the highest 8-bits, where 'color' is the return value of the function 'vga_color()'.
static inline uint16_t vga_entry(uint8_t color, uint8_t character){
	return ( (uint16_t)character | ((uint16_t)color << 8) );
}

//initialize the terminal
void init_terminal(void){
	//set the current terminal row and column positions to 0
	term_row = 0;
	term_col = 0;

	//set the default terminal foreground color to dark grey, and the default terminal background color to black
	global_term_color = vga_color(dark_grey, black);

	//point the frame buffer to address 0xB8000, which is where the frame buffer begins. also, set the width of frame_buffer to 16-bits.
	frame_buffer = (uint16_t*) 0xB8000;
	
	//fill the terminal with empty characters
	for(size_t y = 0; y < TERM_HEIGHT; y++){
		for(size_t x = 0; x < TERM_HEIGHT; x++){
			index = y*TERM_WIDTH + x;
			vga_entry(global_term_color, ' ');
		}
	}
}

//simple function to return the length of a string
size_t strlen(uint8_t* string){
	size_t count = 0;
	while(string[count]){
		count++;
	}
	return count;
}

//function to write a character to the terminal at a specified location
void term_putentryat(size_t x, size_t y, uint8_t color, uint8_t letter){
	size_t index = y*TERM_WIDTH + x;		
	if(letter == '\n'){ term_col++; term_row=0; } 	//rudimentary newline support
	else { frame_buffer[index] = vga_entry(color, letter); }
}

//function to write a string to the terminal
void write_line(uint8_t color, uint8_t* message){
	size_t count = strlen(message);
	for(size_t i = 0; i < count; i++){
		//if the current position of the terminal row is larger than the max position of TERM_WIDTH, start on a new line and reset the current terminal row position.
		if(term_row > TERM_WIDTH){ term_col++; term_row=0; }	
		
		//write the character at the current position
		term_putentryat(term_row, term_col, color, message[i]);

		//increment the terminal row position for every character written
		term_row++;
	}
}

//main kernel function, all it does atm is write the string "Sub to Pewdiepie\nUnsub from T-Series"
extern "C"{
	void k_main(){
		init_terminal();
		write_line(vga_color(dark_grey, black), (uint8_t*)"Sub to ");
		write_line(vga_color(light_blue, black), (uint8_t*)"Pewdiepie\n");
		write_line(vga_color(dark_grey, black), (uint8_t*)"Unsub from ");
		write_line(vga_color(red, black), (uint8_t*)"T-Series.");
	}
}
