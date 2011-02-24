#include <vector>
#include <string>
#include "Sequence.h"

using namespace std;

class scline;
class scdirectiveline;
class scmusicline;
class scinsertionline;
class scchunk;
class scchunklist;
class schilevlist;
class schilevdirective;
class scsetdirective;

class scline
{
	public:
		string linetype;
		virtual string dump() = 0;
};

class scdirectiveline : public scline
{
public:
	scdirectiveline();
	virtual string dump();
};

class scmusicline : public scline
{
private:
	string line;
public:
	string getLine();
	scmusicline(char*);
	virtual string dump();
};

class scinsertionline : public scline
{
private:
	char type;
	string left;
	string op;
	string right;
public:
	scinsertionline(char*);
	scinsertionline(char*, char*, char*);
	virtual string dump();
};

class scchunk
{
private:
	vector<scline*> lines;
	schilevlist* hll;
	string name;
public:
	void addLine(scline*);
	void setName(char*);
	string getName();
	//void resolveReferences(scchunklist*);
	Sequence getSequence(scchunklist*);
	void setHiLevList(schilevlist*);
	string getInstrumentName(bracetype);
	virtual string dump();
};

class scchunklist
{
private:
	vector<scchunk*> chunks;
public:
	void addChunk(scchunk*);
	scchunk* getChunkByName(string chunkname);
	virtual string dump();
};

class schilevlist
{
private:
	vector<schilevdirective*> dirs;
public:
	void addDir(schilevdirective*);
	string getInstrumentName(bracetype);
	virtual string dump();
};

class schilevdirective
{
public:
	string dirtype;
	virtual string dump();
};

class scsetdirective : public schilevdirective
{
private:
	string name;
	bracetype type;
public:
	void setType(bracetype);
	void setName(char*);
	bracetype getType();
	string getName();
	virtual string dump();
};



