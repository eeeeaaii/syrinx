#pragma once
#include "standardincludes.h"


int yyparse(void);
int scyyparse(void);


class Sequencer;
class Module;
class FunctionModule;
struct ymoduleref;
struct ymodule;
struct yfunction;
class ModuleFactory
{
public:
	void setupModules(modGroup&, linkGroup&);
private:
	void initFunctionModule(FunctionModule *fm, yfunction *yf, map<string, string>& assigns);
	void createModules(FunctionModule *fm, ymoduleref *ymr, yfunction *yf, map<string, string>& assigns);
	Module* createLinks(FunctionModule *fm, ymoduleref *ymr, yfunction *yf);
	void addLink(Module* a, Module* b);
	int findInputNumber(ymoduleref *ymr, yfunction *yf);
	int findOutputNumber(ymoduleref *ymr, yfunction *yf);
	Module* getOrCreateModule(ymoduleref *ymr, yfunction *yf, FunctionModule *fm, map<string, string>& assigns);
	Module* getModuleAndLinkAtts(ymoduleref *ymr, yfunction *yf, FunctionModule *fm);
	yfunction* findFunctionByName(string name);
	Module *constructModule(ymodule *ym);
	void initStringArguments(Module* m, ymodule* ym, map<string, string>& assigns);
	ymodule* getYmoduleFromReference(yfunction* yf, ymoduleref* ymr);
	Module* lookupModule(yfunction* yf, string name);

//	Link* wireUp(Module*, Module*, int outnum = 0, int innum = 0);
public:
//	void testinit1(modGroup&, linkGroup&);
//	void testinit2(modGroup&, linkGroup&);
//	void setupSequence1(Sequencer*);
//	void setupSequence2(Sequencer*);
};
