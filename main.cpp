#include "tag.h"
#include "tagreader.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

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

	for(int i = 0; i < argc; i++){
		char* p = "-p";
		if(strcmp(argv[i], p)){
			print_tree(playerNbt, rootCompound);
			return 0;	
		}
	}

	return 0;
}


