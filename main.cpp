#include "tag.h"
#include "tagreader.h"

#include <stdio.h>
#include <stdlib.h>

//The method/function to create the root node in our tree
TagCompound* create_root_tag(FILE *fp)
{
	return readCompoundTag(fp); 	
}

//The main method
int main(int argc, const char * argv[])
{
	FILE *playerNbt;
	playerNbt = fopen("player.nbt","r");

	create_root_tag(playerNbt);

	return 0;
}


