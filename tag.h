#ifndef TAG_H
#define TAG_H

#include <stdlib.h>
#include <vector>
#include <string>

//The base tag class
class Tag
{
	public:
	std::string *name;

	int tagId;

	virtual void printTag();
};

//All of the tags

class TagByte : public Tag
{
	public:
	char data;

	void printTag();
};

class TagShort : public Tag
{
	public:
	short data;

	void printTag();
};

class TagInt : public Tag
{
	public:
	int data;

	void printTag();

};

class TagLong : public Tag 
{
	public:
	long data;
	
	void printTag();

};

class TagFloat : public Tag
{
	public:
	float data;

	void printTag();
};

class TagDouble : public Tag
{
	public:	
	double data;
	
	void printTag();
};

class TagByteArray : public Tag
{
	public:
	int numberOfTags;
	char * data;	

//	~TagByteArray();
//
	void printTag();
};

class TagString : public Tag
{
	public:
	int lengthOfString;
	char * data;

//      ~TagString();
//

	void printTag();
};


class TagList : public Tag
{
	public:
	int tagId;

	int numberOfTags;
	Tag* *tags;

//	~TagList();
	
	void printTag();	
};

class TagCompound : public Tag
{
	public:
	std::vector<Tag*> *tagList;

//	~TagCompound();

	void printTag();
};

class TagIntArray : public Tag
{
	public:
	int sizeOfArray;
	int *intArray;

//	~TagIntArray();
//
	void printTag();
};

class TagLongArray : public Tag
{
	public:
	int sizeOfArray;
	long *longArray;
	
//	~TagLongArray();
//
	void printTag();
};



#endif
