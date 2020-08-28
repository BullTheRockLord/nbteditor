#ifndef DISPLAY_H
#define DISPLAY_H

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>

#include "tag.h"

/*** TERMINAL RAW MODE 
 *   Functions related to using to controlling the terminal raw mode
 * ****/
struct termios orig_termios;

void disableRawMode(){	
	int result = tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios);
	if(result == -1){
		std::cout << "TSET FAIL 1 ";
		exit(13);
	}
	std::cout << "\x1b[2J";
	std::cout << "\x1b[H";
}
void enableRawMode(){
	int result = tcgetattr(STDIN_FILENO, &orig_termios);
	if(result == -1){
		std::cout << "TSET FAIL 2 ";
		exit(13);
	}
	atexit(disableRawMode);	

	struct termios raw;
	tcgetattr(STDIN_FILENO, &raw);

	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_oflag &= ~(OPOST);
	raw.c_iflag &= ~(ICRNL | IXON);
	raw.c_cc[VMIN] = 0.1;
	raw.c_cc[VTIME] = 1;
	result = tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	if(result == -1){
		std::cout << "TSET FAIL 3";
		exit(13);
	}
}
void get_terminal_dimensions(int& height, int& width){
	struct winsize ws;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

	height = ws.ws_col;
       	width = ws.ws_row;	
}

/*** LINE LIST FUNCTIONS
 *   Functions that relate to the linked list structure that represents the lines we print out to the consoel
 *   Each entry in the list relate to the line in the print out, and contains tracking info such as which tag "owns" that printout
 *   The position in the list of each node is the line position - 1, so the 1st node is line 1, the 2nd node is line 2
 * ****/

struct Line{
	std::string *line_string;		
	int name_start;
	int name_end;
	int value_start;
	int value_end;
	Tag *tag_owner;	
};
struct Line *first_line = 0;
struct Line *last_line = 0;
int indentation = 0; //The amount of tabs to at the start of each line list, representing the depth in the tree

/*NBT TREE FUNCTIONS 
* Functions that relate to the taking a nbt tree and generating a our line list from it
*/

void read_nbt_tag(Tag* tag)
{

}
void read_nbt_tree(TagCompound* root_tag)
{
	if(first_line == 0){
		//This will be our root node, and should always be the first entry in the line list ,and the first line of the print out
		first_line = new struct Line;
		last_line = first_line;

		//Set all the values to 0, because none of the aspects of the root tree should be editable
		first_line->name_start = -1;
		first_line->name_end = -1;
		first_line->value_start = -1;
		first_line->value_end = -1;	
	}
	
	std::vector<Tag*> *tag_list = root_tag->tag_list;
	for(int i = 0;i < tag_list->size();i++){
		read_nbt_tag((*tag_list)[i]);
	}
}

/**
 * INTERACTIVE MODE FUNCTIONS
 * functions that relate to the interative mode, using the cursor to delete and modify nodes
 */

void enter_interactive_mode(TagCompound* root_compound)
{
	read_nbt_tree(root_compound);
}

#endif
