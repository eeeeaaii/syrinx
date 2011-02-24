#pragma once
#include "standardincludes.h"
/*
the purpose of this class is to translate the orchestra
from a representation of the source code to the
actual data structures we need.

*/

class OrchFunction;
class FunctionModule;
class OrchTranslator
{
private:
	map<string, OrchFunction*> definedfunctions;

public:
	string dump();
	OrchTranslator();
	void addFunction(OrchFunction *of);
	FunctionModule *translate(string name, Link* note, Link* trig, Link* velo);
	OrchFunction* getFunctionFromName(string name);
};


