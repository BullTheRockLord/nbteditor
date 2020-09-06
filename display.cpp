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
	raw.c_cc[VMIN] = 1;
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
	std::string line_string;		
	int name_start;
	int name_end;
	int value_start;
	int value_end;
	Tag *tag_owner;	

};
std::vector<struct Line> line_vector = std::vector<struct Line>();
int indentation = 0; //The amount of tabs to at the start of each line list, representing the depth in the tree

/*NBT TREE FUNCTIONS 
* Functions that relate to the taking a nbt tree and generating a our line list from it
*/

//Function prototyping
void read_nbt_tree(TagCompound* root_tag);

void  add_indent(std::string & line_string)
{
	for(int i = 0; i < indentation; i++){
		line_string += "\t";	
	}
}

void get_value_string(Tag* tag,std::string & value_string) //Returns the value string for all type of tags except 9,10 (list, compound)
{
	 value_string = tag->value_string;
}

void read_nbt_tag(Tag* tag)
{
	line_vector.push_back(Line());	
	struct Line &line = line_vector.back();

	//Set the owner of the tag, so later we know what tag to change the value/name of
	line.tag_owner = tag;

	//Fill out all the string information for the tag
	line.line_string = std::string();
	add_indent(line.line_string);	

	line.line_string += "\""; 

	line.name_start = line.line_string.length();	//Save the length of the string at this current point as the start of the name 	
	//It is possible for the name to be blank, if so then we can't evaluate the pointer, now can we
	if(tag->name != 0){
		line.line_string += *(tag->name);	
	}	
	line.name_end = line.line_string.length(); //Save the length of the string as the name end

	line.line_string  +=  "\"";

	//Do the tag specific formatting
	switch(tag->tagId){
		//All these type of tags have a very similar type of print out, formated as "name" : "value"
		case 0 ... 8:
		case 11 ... 12:
		{		
			//Set the string value pointed to by line string as that string + the closing quotes for the name + the opening quotes for the next value
			line.line_string += " : \"";	

			//Add the value, and store the line
			line.value_start = line.line_string.length();
			std::string value_string;
			get_value_string(tag, value_string);
			line.line_string += value_string + "\"";
			line.value_end = line.line_string.length();

			if(tag->tagId == 7 || tag->tagId == 11 || tag->tagId == 12){ //If we are one of the array type tags, then we need to account for the { and } 
				line.value_start = line.value_start + 1;	
				line.value_end = line.value_end - 1;
			}
			
			return;
		}
		//9,10 are the compound and list tags, they have a unique format of "name", with the values being lines under it with a higher indent
		case 9 ... 10:
		{
			line.value_start = -1;
			line.value_end = -1;

			if(tag->tagId == 9){
				line.line_string += "TAG LIST";	
				return;
			};			
			if(tag->tagId == 10){
				line.line_string += "TAG Compound";	
				TagCompound* tag_compound = static_cast<TagCompound*>(tag);	
				indentation++;
				read_nbt_tree(tag_compound);
				indentation--;
				return;
			}	
			break;
		}
	}	

}
void read_nbt_tree(TagCompound* root_tag)
{
	if(line_vector.size() == 0){
		//This will be our root node 
		line_vector.push_back(Line());
		struct Line first_line = line_vector.back();

		//Set all the values to 0, because none of the aspects of the root tree should be editable
		first_line.name_start = -1;
		first_line.name_end = -1;
		first_line.value_start = -1;
		first_line.value_end = -1;	

		//Set the string value
		first_line.line_string = std::string("ROOT COMPOUND");
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

int screen_offset = 0;
int screen_height = 0;
int screen_width = 0;
int cursor_y = 0;
int cursor_x = 0;

void refresh_screen()
{
	//Clear the screen
	std::cout << "\x1b[2J";
	std::cout << "\x1b[H";

	//Print out our lines	
	for(int i = screen_offset; i < screen_height + screen_offset;i++){
		if(i >= line_vector.size()){
			break;
		}
		std::cout << i << "- " << line_vector[i].line_string << "\n\r";
	}

	//Move our cursor the the same position as before
	for(int y = 0 ; y < cursor_y;y++){
		std::cout << "\x1b[A";
	}
	for(int x = 0; x < cursor_x;x++){
		std::cout << "\x1b[C";	
	}
}

void enter_interactive_mode(TagCompound* root_compound)
{
	//Generate our line vector	
	read_nbt_tree(root_compound);

	//Enter raw mode, so we can control input and output better
	enableRawMode();
	get_terminal_dimensions(screen_height, screen_width);

	screen_height = 50;

	//Do our inital printout of the screen
	refresh_screen();
	std::cout << "\x1b[H";


	char c;
	while(true){
		std::cin >> c;
		if(c == 'j'){
			if(cursor_y == screen_height){
				screen_offset += 1;
				refresh_screen();	
			}else{
				cursor_y++;
				std::cout << "\x1b[B";
			}	
		}
		if(c == 'k'){
			if(cursor_y == 0){
				if(screen_offset > 0){
					screen_offset -= 1;
					refresh_screen();
				}
			}else{
				std::cout << "\x1b[A";
				cursor_y--;
			}	
		}
		if(c == 'l'){
			int line_number = screen_offset + cursor_y;
			if(line_number >= line_vector.size())
				continue;
			if(line_vector[line_number].line_string.length() <= cursor_x){
				continue;
			}
			cursor_x++;
			std::cout << "\x1b[C";
		}
		if(c == 'h'){
			cursor_x--;
			std::cout << "\x1b[D";
			if(cursor_x < 0){
				cursor_x = 0;	
			}
		}
		if(c == 'q'){
			return;
		}
	}
}

#endif
