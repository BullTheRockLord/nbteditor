#include "tag.h"
#include "tag.h"
#include "tagreader.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

//The method/function to create the root node in our tree
TagCompound* create_root_tag(FILE *fp){	
	//Get rid of the start tag
	//Otherwise we will always be nested one deep too many
	fgetc(fp);
	fgetc(fp);
	fgetc(fp);

	
	//Now we can read the compound tag
	TagCompound* tagCompound = readCompoundTag(fp); 	

	return tagCompound;
}

//Include some symbols from the display translation unit
void enter_interactive_mode(TagCompound* root_compound);

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
			return 0;
		}	
	}

	enter_interactive_mode(rootCompound);

	return 0;
}


