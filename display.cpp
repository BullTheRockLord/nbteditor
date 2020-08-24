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

#endif
