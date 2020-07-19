#ifndef TAG_H
#define TAG_H

//The base tag class
class Tag
{
	public:
	char *name;
	bool singleLineTag;
	bool hasName;

	int tagId;

	virtual char* getLineString();		
	virtual char* parseLineString();
}

//All of the tags

class TagByte : public Tag
{
	char data;
}

class TagShort : public Tag
{
	short data;
}

class TagInt : public Tag
{
	int data;
}

class TagLong : public Tag 
{
	long data;
}

class TagFloat : public Tag
{
	float data;
}

class TagDouble : public Tag
{
	double data;
}

class TagByteArray : public Tag
{
	char * data;	
}

class TagString : public Tag
{
	char * data;
}

class TagList : public Tag
{
	int tagId;
	int numberOfTags;

	Tag* *tags;
		
}

class TagCompound : public Tag
{
	int numberOfTags;
	Tag* *tags;	
		
}

class TagIntArray : public Tag
{
 	int *intArray;
}

class TagLongArray : public Tag
{
	long *longArray;
}



#endif
