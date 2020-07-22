#include "tag.h"
#include "tagreader.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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

	std::cout << "Root tag created fine" << std::endl;
	return tagCompound;
}

//The main method
int main(int argc, const char * argv[])
{
	FILE *playerNbt;
	playerNbt = fopen("player.nbt","r");

	create_root_tag(playerNbt);

	return 0;
}


