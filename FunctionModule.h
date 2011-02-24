#pragma once
#include "Module.h"

class Link;
class FunctionModule : public Module
{
private:
	int input_counter;
	int output_counter;
	map<int, Module*> inner_inputs;
	map<int, Module*> inner_outputs;
	struct paraminit;
	map<string, paraminit*> inner_linked_attributes;
protected:
	modGroup inner_modules;
	linkGroup inner_links;
	string name;
public:
	FunctionModule(string);
	void processOneSample();
	map<string, Module*> realmods;
	Module* getNextInnerInput();
	Module* getNextInnerOutput();
	void setNthInnerInput(int n, Module* m);
	void setNthInnerOutput(int n, Module* m);
	void setInnerLinkedAttribute(string name, string modname, Module *m);
	void initInnerLinkedAttribute(string name, Link* l);
	virtual int getMaxOutputs() { return 999;}
	virtual int getMaxInputs() { return 999; }
	virtual string nameDebug() { return string("functionmodule"); }
	virtual string getInfo();
	virtual void trace();
	void addModule(Module* m);
	void addLink(Link* l);
	virtual void finalize();
};

struct FunctionModule::paraminit
{
		string name;
		Module *mod;
};
