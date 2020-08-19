#ifndef DISPLAY_H
#define DISPLAY_H

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>

#include "tag.h"

/*** TERMINAL RAW MODE ****/
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

/**PRINTING TRACKERS**/
int line_number = 0;
int tree_depth = 0;

int terminal_height = 0;
int terminal_width = 0;

int terminal_offset = 0; //The offset down ( scrolled lines down)

/**TERMINAL CONTROL FUNCTIONS**/
void editorRefreshScreen(){
	std::cout << "\x1b[2J";
	std::cout << "\x1b[H";

	line_number = 0;
	tree_depth = 0;

	if(terminal_height == 0){
		get_terminal_dimensions(terminal_height, terminal_width);
	}
}

/**USER INPUT METHODS**/
void process_display_input(std::string input){ //Display module gets to process any input 
	if(input == "j"){
		terminal_offset += 1;
		if(terminal_offset >= (terminal_height + 10)){
			terminal_offset -= 1;
		}
	}
	if(input == "k"){
		terminal_offset -= 1;
		if(terminal_offset < 0){
			terminal_offset += 1;	
		}
	}
}

/**PRINTING METHODS DECLERATION**/
void printNbtTree(TagCompound* rootTag);

/**PRINTING METHODS DEFINITIONS**/
void printLineStart(){

	std::cout << line_number;	

	short number_length = 1;
	if(line_number > 10){
		number_length++;	
	}
	if(line_number > 100){
		number_length++;	
	}

	for(short i = 0; i < (4 - number_length);i++){
		std::cout << " ";
	}

	for(short i = 0; i < tree_depth ; i++){
		std::cout << "    ";	
	}
}

void printNbtTag(Tag* tag){

	bool do_print = true;

	if(line_number > terminal_height){
		return;	
	}
	if(line_number < terminal_offset){
		do_print = false;
	}	

	//Special case where we have a tag end, we should never print that
	if(tag->tagId == 0){
		return;
	}

	line_number++;
	if(do_print)	
		printLineStart();		

	switch(tag->tagId){
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			if(do_print)
				std::cout << tag->getLineString() << std::endl << "\r";
			break;
		case 7:
			if(do_print)
				std::cout << std::string("TAG BYTE ARRAY") << std::endl << "\r";	
		case 8:
			if(do_print)
				std::cout << tag->getLineString() << std::endl << "\r";	
			break;
		case 9:
			if(do_print)
				std::cout << std::string("TAG LIST") << std::endl << "\r";
			break;
		case 10:
			if(do_print)
				std::cout << tag->getLineString() << std::endl << "\r";
			printNbtTree((TagCompound*)tag);
			break;
		case 11:
			if(do_print)
				std::cout << tag->getLineString() << std::endl << "\r";
			break;
		case 12:
			if(do_print)
				std::cout << std::string("TAG LONG ARRAY") << std::endl << "\r";
			break;

	}


}
void printNbtTree(TagCompound* rootTag){	
	//Special case for the master root tag of the whole tree	
	if(line_number == 0){
		line_number++;
		if(terminal_offset < 1){
			printLineStart();
			std::cout << "Root Tag {" << rootTag->tagList->size() << "}" << std::endl << "\r";
		}
	}
		
	std::vector<Tag*> *tagList = rootTag->tagList;
	tree_depth++;
	for(int i = 0; i < tagList->size();i++){
		Tag* tag_pointer = (*(tagList))[i];
		printNbtTag(tag_pointer);	
	}
	tree_depth--;
}

#endif
