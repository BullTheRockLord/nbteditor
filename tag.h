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

	std::string value_string;

	int tagId;
};

//All of the tags

class TagByte : public Tag
{
	public:
	char data;

};

class TagShort : public Tag
{
	public:
	short data;
};

class TagInt : public Tag
{
	public:
	int data;
	
};

class TagLong : public Tag 
{
	public:
	long data;
};

class TagFloat : public Tag
{
	public:
	float data;
};

class TagDouble : public Tag
{
	public:	
	double data;
};

class TagByteArray : public Tag
{
	public:
	std::vector<char> *char_array;	

//	~TagByteArray();
};

class TagString : public Tag
{
	public:
	std::string* data;

//      ~TagString();
};


class TagList : public Tag
{
	public:
	std::vector<Tag*> *tag_list;

//	~TagList();
};

class TagCompound : public Tag
{
	public:
	std::vector<Tag*> *tag_list;

//	~TagCompound();
	
};

class TagIntArray : public Tag
{
	public:
	std::vector<int> *int_array;

//	~TagIntArray();
};

class TagLongArray : public Tag
{
	public:
	std::vector<long> *long_array;

//	~TagLongArray();
//
	
};



#endif
