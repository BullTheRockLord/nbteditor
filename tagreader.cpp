#include "tag.h"
#include "tagreader.h"

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

//Methods that read data in big endian and convert it to little endian
short readShort(FILE *fp){
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
	
	for(int i = 0; i < 8; i++){
		ret = ret << 8;
		ret += (short) fgetc(fp);
	}	

	return ret;
}

float readFloat(FILE *fp){	
	char *floatIn = (char*)malloc(sizeof(char) * 4);
	char *floatOut = (char*)malloc(sizeof(char) * 4);

	int readValues = fread(floatIn, 1, 4, fp);

	if(floatIn == NULL){
		std::cout << "MEMORY ALLOCATION FAIL";
		exit(13);
	}

	for(int i = 0; i < 4;i++){
		floatOut[4 - i - 1] = floatIn[i]; //Flip the bytes
	}	

	float *ret = (float *) floatOut;
	
	return *ret;	
}

double readDouble(FILE *fp){	
	char *doubleIn = (char*)malloc(sizeof(char) * 8);
	char *doubleOut = (char*)malloc(sizeof(char) * 8);

	int readValues = fread(doubleIn, 1, 8, fp);

	if(doubleOut == NULL){
		std::cout << "MEMORY ALLOCATION FAIL";
		exit(13);
	}

	for(int i = 0; i < 8;i++){
		doubleOut[8 - i - 1] = doubleIn[i]; //Flip the bytes 
	}	

	double *ret = (double *) doubleOut;
	
	return *ret;	
}

//Read tag header method
void readTagHeader(FILE *fp, int& tagId, std::string*& name){
	char nextTagId = fgetc(fp); //The tag id is stored in a byte
	tagId = nextTagId; //Convert it to a number

	if(tagId == 0){
		return;
	}
	//if(tagId == -1){
	//	tagId = 0;
	//	return;
//	}	

	short lengthOfName = readShort(fp);

	if(feof(fp) == true){
		std::cout << "END OF FILE" << std::endl;
		std::cout << tagId;	
	}

	if(lengthOfName < 0){
		std::cout << "NAME SIZE READ AS NEGATIVE, BAD!";
		exit(12);	
	}

	name = new std::string();	

	if( lengthOfName > 0){
		for(short i = 0; i < lengthOfName;i++){
			char nextCharacter = fgetc(fp);
			name->push_back(nextCharacter);	
		}
	}else{
		name = new std::string(); 
	}
	
		

	return;
}

//Read tag payload methods
TagByte* readByteTag(FILE *fp){	
	TagByte *tagByte = new TagByte();
	tagByte->data = fgetc(fp);

	tagByte->value_string = std::string(1, (tagByte->data));

	return tagByte;	
}
TagShort* readShortTag(FILE *fp){
	TagShort* tagShort = new TagShort();
	tagShort->data = readShort(fp);

	tagShort->value_string = std::to_string((tagShort->data));

	return tagShort;
}
TagInt* readIntTag(FILE *fp){
	TagInt *tagInt = new TagInt();
	tagInt->data = readInt(fp);

	tagInt->value_string = std::to_string((tagInt->data));

	return tagInt;
}
TagLong* readLongTag(FILE *fp){
	TagLong *tagLong = new TagLong();
	tagLong->data = readLong(fp);

	tagLong->value_string = std::to_string((tagLong->data));

	return tagLong;
}
TagFloat* readFloatTag(FILE *fp){
	TagFloat *tagFloat = new TagFloat();
	tagFloat->data = readFloat(fp);

	tagFloat->value_string = std::to_string((tagFloat->data));

	return tagFloat;
}
TagDouble* readDoubleTag(FILE *fp){
	TagDouble *tagDouble = new TagDouble();
	tagDouble->data = readDouble(fp);

	tagDouble->value_string = std::to_string((tagDouble->data));

	return tagDouble;
}
TagByteArray* readByteArrayTag(FILE *fp){
	TagByteArray *tagByteArray = new TagByteArray();

	int sizeOfArray = readInt(fp);	

	tagByteArray->char_array = new std::vector<char>();
	for(int i = 0; i < sizeOfArray; i++){
		(*(tagByteArray->char_array))[i] = fgetc(fp);
	}

	return tagByteArray;
}
TagString* readStringTag(FILE *fp){
	TagString* tagString = new TagString();
	
	short sizeOfString = readShort(fp);
	std::string data = std::string(); 

	for(int i = 0; i < sizeOfString;i++){
		data = data + std::string(1,fgetc(fp));
	}

	tagString->data = new std::string(data);
	tagString->value_string = *(tagString->data);

	return tagString;
}
TagList* readListTag(FILE *fp){
		
	char tagId = fgetc(fp);		
	int numberOfTags = readInt(fp);
	
	std::vector<Tag*> *tag_vector = new std::vector<Tag*>();
	
	TagList* tagList = new TagList();
	tagList->tag_list = tag_vector;	

	for(int i = 0; i < numberOfTags;i++){
		switch (tagId) {
			case 0:
				std::cout << "ERROR IN TAG LIST, END TAG FOUND" << std::endl;
				exit(13);
			case 1:
				tag_vector->push_back(readByteTag(fp));					
				break;
			case 2:
				tag_vector->push_back(readShortTag(fp));		
				break;
			case 3:
				tag_vector->push_back(readIntTag(fp));		
				break;
			case 4:
				tag_vector->push_back(readLongTag(fp));		
				break;
			case 5:
				tag_vector->push_back(readFloatTag(fp));		
				break;
			case 6:
				tag_vector->push_back(readDoubleTag(fp));		
				break;
			case 7:
				tag_vector->push_back(readByteArrayTag(fp));		
				break;
			case 8:
				tag_vector->push_back(readStringTag(fp));		
				break;
			case 9:
				tag_vector->push_back(readListTag(fp));		
				break;
			case 10:
				tag_vector->push_back(readCompoundTag(fp));		
				break;
			case 11:
				tag_vector->push_back(readIntArrayTag(fp));		
				break;
			case 12:
				tag_vector->push_back(readLongArrayTag(fp));		
				break;

		}		
		tag_vector->back()->tagId = tagId;
	}
	return tagList;
}
TagCompound* readCompoundTag(FILE *fp){

	TagCompound* tagCompound = new TagCompound();

	int tagId = 0;
	std::string *tagName = NULL;

	std::vector<Tag*> *tagList = new std::vector<Tag*>();	

	do {
		readTagHeader(fp, tagId, tagName);

		Tag* tag;

		switch (tagId){
			case 0:
				tag = new Tag();
				break;
			case 1:
				tag = readByteTag(fp);
				break;
			case 2:
				tag = readShortTag(fp);
				break;
			case 3:
				tag = readIntTag(fp);
				break;
			case 4:
				tag = readLongTag(fp);
				break;
			case 5:
				tag = readFloatTag(fp);
				break;
			case 6:
				tag = readDoubleTag(fp);
				break;
			case 7:
				tag = readByteArrayTag(fp);
				break;	
			case 8:
				tag = readStringTag(fp);
				break;
			case 9:
				tag = readListTag(fp);
				break;
			case 10:
				tag = readCompoundTag(fp);
				break;
			case 11:
				tag = readIntArrayTag(fp);
				break;
			case 12:
				tag = readLongArrayTag(fp);
				break;
		}

		tag->tagId = tagId;
		tag->name = tagName;

		tagList->push_back(tag);

	} while(tagId != 0);

	tagCompound->tag_list = tagList;
	tagCompound->name = new std::string();

	return tagCompound;
}

TagIntArray* readIntArrayTag(FILE *fp){
	TagIntArray* tagIntArray = new TagIntArray();

	int sizeOfArray = readInt(fp);
	
	tagIntArray->int_array = new std::vector<int>();
	for(int i = 0; i < sizeOfArray;i++){
		tagIntArray->int_array->push_back(readInt(fp));
	}

	return tagIntArray;
}
TagLongArray* readLongArrayTag(FILE *fp){
	TagLongArray* tagLongArray = new TagLongArray();

	int sizeOfArray = readInt(fp);

	tagLongArray->long_array = new std::vector<long>();	
	
	for(int i = 0; i < sizeOfArray;i++){
		tagLongArray->long_array->push_back(readLong(fp));
	}

	return tagLongArray;
}


