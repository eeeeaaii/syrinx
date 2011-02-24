#pragma once
#include "standardincludes.h"
class Module;
class Link
{
private:
	static int nextid;
	double in;
	double out;
	bool newval;
	Module* output;
	Module* input;
public:
	Link();
	Link(double val);
	void putData(double);
	double getData(void);
	void propagate();
	bool hasInput();
	bool hasOutput();
	void setInput(Module *m);
	void setOutput(Module *m);
	Module *getInput();
	Module *getOutput();
	string id;
};

