#pragma once
#include "standardincludes.h"
#include "Module.h"

#define BUFSIZE 64

class FormatChunk {
public:
	char chunkID[4];
	long chunksize;

	short formattag;
	unsigned short channels;
	unsigned long samplespersec;
	unsigned long avgbytespersec;
	unsigned short blockalign;
	unsigned short bitspersample;

	FormatChunk();
};

class DataChunk
{
public:
	char chunkID[4];
	long size;
	DataChunk(long s);
};


class FileOut : public Module
{
protected:
	virtual int getMaxOutputs() { return 0;}
	virtual int getMaxInputs() { return 999; }
	FILE* fp;
	string filename;
public:
	FileOut();
	~FileOut();
	virtual void processOneSample();
	virtual string nameDebug() { return string("fileout"); }
	virtual void setStringParameter(string pname, string value);
	virtual string getInfo();
	void finalize();
};

