#include "tag.h"

#include <stdlib.h>

//Implementing all our methods for getting line strings

TagCompound::~TagCompound(){
	for(int i = 0;i < numberOfTags;i++){
		delete *(tags + i);
	}
	free(tags);
}


