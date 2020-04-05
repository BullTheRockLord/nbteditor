/**
	Command line interface for working with NBT files, version 0.1
**/

#include <stdio.h>
#include <stdlib.h>

//Utility methods for reading data

unsigned short readShort(FILE *fp){
	char byteOne = fgetc(fp);
	char byteTwo = fgetc(fp);

	unsigned short ret = (short) byteOne;
	ret = ret << 8;

	ret += (short) byteTwo;	
	
	return ret;
	
}

int readInt(FILE *fp){

	unsigned int ret = 0;
	
	for(int i = 0; i < 4; i++){
		ret = ret << 8;
		ret += (short) fgetc(fp);
	}	

	return ret;

}

long readLong(FILE *fp){
	unsigned long ret = 0;
	
	for(int i = 0; i < 4; i++){
		ret = ret << 8;
		ret += (short) fgetc(fp);
	}	

	return ret;
}

float readFloat(FILE *fp){
	
	char *floatIn = malloc(sizeof(char) * 4);
	char *floatOut = malloc(sizeof(char) * 4);

	int readValues = fread(floatIn, 1, 4, fp);

	if(floatIn == NULL){
		printf("MEMORY ALLOCATION ERROR");
		exit(13);
	}

	for(int i = 0; i < 4;i++){
		floatOut[4 - i - 1] = floatIn[i];
	}	

	float *ret = floatOut;
	
	return *ret;
	
}

double readDouble(FILE *fp){
	
	char *doubleIn = malloc(sizeof(char) * 8);
	char *doubleOut = malloc(sizeof(char) * 8);

	int readValues = fread(doubleIn, 1, 8, fp);

	if(doubleOut == NULL){
		printf("MEMORY ALLOCATION ERROR");
		exit(13);
	}

	for(int i = 0; i < 8;i++){
		doubleOut[8 - i - 1] = doubleIn[i];
	}	

	double *ret = doubleOut;
	
	return *ret;
	
}

//Methods for keeping track of data indentsk

int indentLevel = 0;

void printIndent(){
	for(int i = 0; i < indentLevel;i++){
		printf("     ");	
	}
}

//Some function prototyping to allow easier organization

int nextTag(FILE *fp); 
int readPayloadOfTag(int id, FILE *fp);

//All of our tag methods, each one process a payload, then returns

int tag_byte(FILE *fp){
	char in = fgetc(fp);

	printf("%d", in);

	return 0;
}

int tag_short(FILE *fp){
	short payload = readShort(fp);

	printf("%d", payload);

	return 0;
}

int tag_int(FILE *fp){
	int payload = readInt(fp);

	printf("%d", payload);
}

int tag_long(FILE *fp){
	long payload = readInt(fp);

	printf("%ld", payload);
}

int tag_float(FILE *fp){
	float in = readFloat(fp);
	printf("%.2f", in);
}

int tag_double(FILE *fp){	
	double in = readDouble(fp);
	printf("%.2f", in);	
}

int tag_byte_array(FILE *fp){
	int length = readInt(fp);

	if(length == 0){
		printf("[]");
		return 0;
	}
	
	printf("[");

	for(int i = 0; i < length; i++){
		printIndent();

		short o = 0;
		char in = fgetc(fp);

		o = o + in;

		printf("%d\n",o);	
		
		if(i != length - 1){
			printf(",");
		}

	}

	printf("]\n");
	return 0;
}

int tag_string(FILE *fp){

	
	short stringLength = readShort(fp);


	char *name = malloc(stringLength);
	char *nameStart = name;

	if(name == NULL){
		printf("Failed memory allocation\n");
		exit(0);
	}

	for(short i = 0; i < stringLength;i++){
		char nextCharacter = fgetc(fp);	
		*name = nextCharacter;
		name += 1;
	}

	printf("\"%s\"", nameStart);

}

int tag_list(FILE *fp){
	int tagId = (short) fgetc(fp);

	int length = readInt(fp);

	if(length == 0){
		printf("[]");
		return 0;
	}
	indentLevel += 1;
	printf("\n");
	for(int i = 0;i < length;i++){
		printIndent();
		readPayloadOfTag(tagId, fp);		
		if(tagId != 10 && length - 1 != i){
			printf("\n");
		}
	}	
	indentLevel -= 1;
}

int tag_compound(FILE *fp){

	printf("\n");
	indentLevel += 1;

	int tag = -10;
	do{
		printIndent();
		tag = nextTag(fp);

		if(tag == 0){
			printf("END TAG\n");
		}

		if(tag != 10 && tag != 0){
			printf("\n");
		}

	}while(tag != 0);

	indentLevel -= 1;
	
	return 0;
}

int tag_int_array(FILE *fp){
	int length = readInt(fp);

	if(length == 0){
		printf("[]");
		return 0;
	}
	
	indentLevel += 1;
	
	printf("[");

	for(int i = 0;i<length;i++){
		int payload = readInt(fp);
		if(i != length - 1){
			printf("%d,", payload);
		}else{
			printf("%d",payload);
		}	
	}

	printf("]");

	indentLevel -= 1;

	return 0;	
}

int tag_long_array(FILE *fp){
	int length = readInt(fp);
	
	if(length == 0){
		printf("[]");
		return 0;
	}

	indentLevel += 1;
	
	printf("[");

	for(int i = 0;i<length;i++){
		long payload = readLong(fp);
		if(i != length - 1){
			printf("%ld,", payload);	
		}else{
			printf("%ld", payload);
		}
	}

	printf("]");
	indentLevel -= 1;

	return 0;
} 

//This is where we store our parallel array structure for prototyping the functions
#define numberOfTags 13

int (*tagMethods[numberOfTags])(FILE *fp) = {
	NULL,
	&tag_byte,
	&tag_short,
	&tag_int,
	&tag_long,
	&tag_float,
	&tag_double,
	&tag_byte_array,
	&tag_string,
	&tag_list,
	&tag_compound,
	&tag_int_array,
	&tag_long_array
};

//This is our method for handling the naming of tags
char *processName(FILE *fp){
	char firstByte = fgetc(fp);
	char secondByte = fgetc(fp);

	unsigned short stringLength = firstByte;
	stringLength = stringLength << 8;

	stringLength += (short) secondByte;

	char *name = malloc(stringLength);
	char *nameStart = name;

	if(name == NULL){
		printf("Failed memory allocation\n");
		exit(0);
	}

	for(short i = 0; i < stringLength;i++){
		char nextCharacter = fgetc(fp);	
		*name = nextCharacter;
		name += 1;
	}

	return nameStart;
	
}

//Primary method for getting a next tag, can recursively be called by tag handler methods, return is the tag id
int nextTag(FILE *fp){
	short nextTagId = fgetc(fp);

	if(nextTagId == 0){ // If end tag, then we can never have a string or a payload
		return 0;
	}

	
	char* name = processName(fp);

	printf("%s : ", name);

	tagMethods[nextTagId](fp);		

	return nextTagId;
}

//This is a secondary method for processing tags without ids
int readPayloadOfTag(int id, FILE *fp){
	
	tagMethods[id](fp);	

	return 0;		
}

//Methods for user command line inteface

int main(int argc, char* *argv){
	FILE *playerNbt;

	playerNbt = fopen("player.nbt", "r");

	printf("Started\n");
	
	nextTag(playerNbt);	

}
