#ifndef DISPLAY_H
#define DISPLAY_H

#include <termios.h>
#include <unistd.h>

#include <iostream>

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
/**TERMINAL CONTROL FUNCTIONS**/
void editorRefreshScreen(){
	std::cout << "\x1b[2J";
	std::cout << "\x1b[H";
}

/**PRINTING TRACKERS**/
int line_number = 0;
int tree_depth = 0;

/**PRINTING METHODS**/
void printNBTTag(Tag* tag){
	line_number++;

	switch(tag->tagId){
		case 0:
			std::cout << "~ EMPTY TAG, SHOULD NOT PRINT"
		case 1:

		case 2:

		case 3:

		case 4:

		case 5:

		case 6:

		case 7:

		case 9:

		case 10:

		case 11:

		case 12:

	}


}
void printNBTTree(TagCompound* rootTag){
	
}

#endif
