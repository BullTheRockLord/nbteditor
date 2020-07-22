#ifndef TAG_READER_H
#define TAG_READER_H

#include "tag.h"

#include <stdio.h>
#include <stdlib.h>

//Methods that read data in big endian and convert it to little endian
short readShort(FILE *fp);
int readInt(FILE *fp);
long readLong(FILE *fp);
float readFloat(FILE *fp);
double readDouble(FILE *fp);

//Indvidual read tag header method
void readTagHeader(FILE *fp, int& tagId, char*& name);

//Indvidual read tag date methods
TagByte* readByteTag(FILE *fp);
TagShort* readShortTag(FILE *fp);
TagInt* readIntTag(FILE *fp);
TagLong* readLongTag(FILE *fp);
TagFloat* readFloatTag(FILE *fp);
TagDouble* readDoubleTag(FILE *fp);
TagByteArray* readByteArrayTag(FILE *fp);
TagString* readStringTag(FILE *fp);
TagList* readListTag(FILE *fp);
TagCompound* readCompoundTag(FILE *fp);
TagIntArray* readIntArrayTag(FILE *fp);
TagLongArray* readLongArrayTag(FILE *fp);

//Master read method
Tag readTag(FILE *fp);

#endif
