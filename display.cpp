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

	struct Line *next_line;
};
struct Line *first_line = 0;
struct Line *last_line = 0;
int indentation = 0; //The amount of tabs to at the start of each line list, representing the depth in the tree

/*NBT TREE FUNCTIONS 
* Functions that relate to the taking a nbt tree and generating a our line list from it
*/

//Function prototyping
void read_nbt_tree(TagCompound* root_tag);

std::string* add_indent(std::string *line_string)
{
	for(int i = 0; i < indentation; i++){
		(*(line_string)) = (*(line_string)) + std::string("\t");
	}
	return line_string;
}

std::string* get_value_string(Tag* tag) //Returns the value string for all type of tags except 9,10 (list, compound)
{
	return new std::string("value");
}

void read_nbt_tag(Tag* tag)
{
	struct Line *line = new struct Line;
	line->next_line = 0;

	//Set the owner of the tag, so later we know what tag to change the value/name of
	line->tag_owner = tag;

	//Fill out all the string information for the tag
	std::string* line_string = new std::string();
	line_string = add_indent(line_string);
	(*(line_string)) = (*(line_string)) + "\"";	

	line->name_start = line_string->length();	//Save the length of the string at this current point as the start of the name 	
	(*(line_string)) = (*(line_string)) + (*(tag->name));	//Add the name
	line->name_end = line_string->length(); //Save the length of the string as the name end

	(*(line_string)) = (*(line_string)) + "\"";

	//Do the tag specific formatting
	switch(tag->tagId){
		//All these type of tags have a very similar type of print out, formated as "name" : "value"
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 11:
		case 12:
		{		
			(*(line_string)) = (*(line_string)) + " : \"";	

			line->value_start = line_string->length();
			(*(line_string)) = (*(line_string)) + (*(get_value_string(tag)));
			line->value_end = line_string->length();

			if(tag->tagId == 7 || tag->tagId == 11 || tag->tagId == 12){ //If we are one of the array type tags, then we need to account for the { and } 
				line->value_start = line->value_start + 1;	
				line->value_end = line->value_end - 1;
			}
			
			//Add the line to our linked list
			last_line->next_line = line;	
			last_line = line;

			return;
		}
		//9,10 are the compound and list tags, they have a unique format of "name", with the values being lines under it with a higher indent
		case 9:
		case 10:
		{
			last_line->next_line = line;
			last_line = line;

			line->value_start = -1;
			line->value_end = -1;

			if(tag->tagId == 9){
				(*(line_string)) = (*(line_string)) + "TAG LIST";	
				return;
			};			
			if(tag->tagId == 10){
				(*(line_string)) = (*(line_string)) + "TAG Compound";	
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
	enableRawMode();
	struct Line *current_line = first_line;	
	while(current_line->next_line != 0){
		std::cout << current_line->line_string << std::endl << "\r";
		current_line = current_line->next_line;
	}
}

#endif
