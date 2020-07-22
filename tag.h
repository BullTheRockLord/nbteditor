#ifndef TAG_H
#define TAG_H

//The base tag class
class Tag
{
	public:
	char *name;
	bool singleLineTag;

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
	int numberOfTags;
	char * data;	

//	~TagByteArray();
};

class TagString : public Tag
{
	public:
	int lengthOfString;
	char * data;

//      ~TagString();
};

class TagList : public Tag
{
	public:
	int tagId;

	int numberOfTags;
	Tag* *tags;

//	~TagList();
		
};

class TagCompound : public Tag
{
	public:
	int numberOfTags;
	Tag* *tags;	
	
	~TagCompound();
};

class TagIntArray : public Tag
{
	public:
	int sizeOfArray;
	int *intArray;

//	~TagIntArray();
};

class TagLongArray : public Tag
{
	public:
	int sizeOfArray;
	long *longArray;
	
//	~TagLongArray();
};



#endif
