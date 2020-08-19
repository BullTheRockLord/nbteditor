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
	virtual std::string getLineString();
};

//All of the tags

class TagByte : public Tag
{
	public:
	char data;

	void printTag();
	std::string getLineString();
};

class TagShort : public Tag
{
	public:
	short data;

	void printTag();
	std::string getLineString();
};

class TagInt : public Tag
{
	public:
	int data;

	void printTag();
	std::string getLineString();
};

class TagLong : public Tag 
{
	public:
	long data;
	
	void printTag();
	std::string getLineString();
};

class TagFloat : public Tag
{
	public:
	float data;

	void printTag();
	std::string getLineString(); 
};

class TagDouble : public Tag
{
	public:	
	double data;
	
	void printTag();
	std::string getLineString();
};

class TagByteArray : public Tag
{
	public:
	std::vector<char> *char_array;	

//	~TagByteArray();
//
	void printTag();
	std::string getLineString();
};

class TagString : public Tag
{
	public:
	std::string* data;

//      ~TagString();
//

	void printTag();
	std::string getLineString();
};


class TagList : public Tag
{
	public:
	int tagId;

	int numberOfTags;
	Tag* *tags;

//	~TagList();
	
	void printTag();	
	std::string getLineString();
};

class TagCompound : public Tag
{
	public:
	std::vector<Tag*> *tagList;

//	~TagCompound();

	void printTag();
	std::string getLineString();
};

class TagIntArray : public Tag
{
	public:
	std::vector<int> *int_array;

//	~TagIntArray();
//
	void printTag();
	std::string getLineString();
};

class TagLongArray : public Tag
{
	public:
	int sizeOfArray;
	long *longArray;
	
//	~TagLongArray();
//
	void printTag();
	std::string getLineString();
};



#endif
