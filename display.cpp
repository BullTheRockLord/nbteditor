#ifndef DISPLAY_H
#define DISPLAY_H

#include <termios.h>
#include <unistd.h>

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

/**PRINTING TRACKERS**/
int line_number = 0;
int tree_depth = 0;

/**TERMINAL CONTROL FUNCTIONS**/
void editorRefreshScreen(){
	std::cout << "\x1b[2J";
	std::cout << "\x1b[H";

	line_number = 0;
	tree_depth = 0;
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
	line_number++;
	printLineStart();	

	if(line_number > 40){
		return;	
	}

	switch(tag->tagId){
		case 0:
			std::cout << "~ EMPTY TAG, SHOULD NOT PRINT";
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			std::cout << tag->getLineString() << std::endl << "\r";
			break;
		case 7:
			std::cout << std::string("TAG BYTE ARRAY") << std::endl << "\r";
		case 8:
			std::cout << tag->getLineString() << std::endl << "\r";
			break;
		case 9:
			std::cout << std::string("TAG LIST") << std::endl << "\r";
			break;
		case 10:
			std::cout << tag->getLineString() << std::endl << "\r";
			printNbtTree((TagCompound*)tag);
		case 11:
			std::cout << std::string("TAG INT ARRAY") << std::endl << "\r";
			break;
		case 12:
			std::cout << std::string("TAG LONG ARRAY") << std::endl << "\r";
			break;

	}


}
void printNbtTree(TagCompound* rootTag){
	std::vector<Tag*> *tagList = rootTag->tagList;
	tree_depth++;
	for(int i = 0; i < tagList->size();i++){
		Tag* tag_pointer = (*(tagList))[i];
		printNbtTag(tag_pointer);	
	}
	tree_depth--;
}

#endif
