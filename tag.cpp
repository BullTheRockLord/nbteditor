#include "tag.h"

#include <stdlib.h>
#include <iostream>

//Helper methods for the print line strings
int indent = 0;
void printIndent(){
	for(int i = 0; i < indent;i++){
		std::cout << "     ";
	}
}


//Implementing all our methods for getting line strings
void Tag::printTag()
{
	printIndent();
	std::cout << "WRONG TAG" << std::endl;
}

void TagByte::printTag()
{
	printIndent();
	std::cout << "\"" << *name << "\"" << " : " << this->data << std::endl;
}

void TagShort::printTag()
{	
	printIndent();
	std::cout << "\"" << *name << "\"" << " : " << this->data << std::endl;
}

void TagInt::printTag()
{
	printIndent();
	std::cout << "\"" << *name << "\"" << " : " << this->data << std::endl;
}

void TagLong::printTag()
{
	printIndent();
	std::cout << "\"" << *name << "\"" << " : " << this->data << std::endl;
}

void TagFloat::printTag()
{
	printIndent();
	std::cout << "\"" << *name << "\"" << " : " << this->data << std::endl;
}

void TagDouble::printTag()
{
	printIndent();
	std::cout << "\"" << *name << "\"" << " : " << this->data << std::endl;
}

void TagByteArray::printTag()
{
	printIndent();
}

void TagString::printTag()
{
	printIndent();
}

void TagList::printTag()
{
	printIndent();	
}

void TagCompound::printTag(){
	printIndent();
	indent++;
	std::cout << "\"" << *name << "\"" << " : {" << this->tagList->size() << "}" << std::endl;
	for(int i = 0; i < this->tagList->size();i++){	
		Tag* tag = (*(this->tagList))[i];
		if(tag->tagId == 0){
			printIndent();
			std::cout << "END TAG" << std::endl;
			continue;
		}
		if(tag == NULL){
			std::cout << "NULL POINTER TO THE TAG" << std::endl;
		}else{
			tag->printTag();	
		}	
	}
	indent--;	
}

void TagIntArray::printTag(){
	printIndent();
}


void TagLongArray::printTag(){
	printIndent();
}


