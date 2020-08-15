#include "tag.h"
#include "tagreader.h"
#include "display.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

//The method/function to create the root node in our tree
TagCompound* create_root_tag(FILE *fp)
{	
	//Get rid of the start tag
	//Otherwise we will always be nested one deep too many
	fgetc(fp);
	fgetc(fp);
	fgetc(fp);

	//Now we can read the compound tag
	TagCompound* tagCompound = readCompoundTag(fp); 	

	return tagCompound;
}

//The print method
void print_tree(FILE *fp, TagCompound* rootTag){
	rootTag->printTag();
}

//The main method
int main(int argc, const char * argv[])
{	
	FILE *playerNbt;
	playerNbt = fopen("player.nbt","r");

	TagCompound* rootCompound = create_root_tag(playerNbt);

	std::string print_flag = "-p"; 
	for(int i = 0; i < argc; i++){
		std::string this_arg = std::string(argv[i]);
		if(this_arg.compare(print_flag) == 0){
			print_tree(playerNbt, rootCompound);
			return 0;
		}	
	}

	enableRawMode();	

	char c;
	while(true){
		editorRefreshScreen();
		printNbtTree(rootCompound);
		std::cin >> c;
		if(!iscntrl(c)){
		}	
		if(c == 'q'){
			break;
		}
	}

	return 0;
}


