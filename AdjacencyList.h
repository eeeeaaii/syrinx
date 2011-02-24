#pragma once
#include "standardincludes.h"

/*** ALNode ***/

class ALNode
{
private:
	string name;
public:
	string getName();
	void setName(string n);
};

/*** ALEdge ***/

class ALEdge
{
private:
	string jackName;
public:
	string getJackName();
	void setJackName(string jn);
};

/*** ALEntry ***/

class ALEntry
{
private:
	ALNode *node;
	vector<ALEdge*> edges;
};

/*** AdjacencyList ***/

class AdjacencyList
{
private:
	vector<ALEntry*> list;

public:
	AdjacencyList();
	void addNode(string name);
	void connectToInput(string nodename1, string nodename2);
	void connectToJack(string nodename1, string nodename2, string jackname);
};

