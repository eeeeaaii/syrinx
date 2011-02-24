#pragma once
#include "standardincludes.h"


/*
Two symbols cannot point to the same module, i.e.
this is illegal, but can be caught by yacc:

@a : @b

One symbol cannot point to two modules -- this is
also illegal, but can't be caught by yacc:

@a: {osc foo="bar"}
@a: {filt foo2="bar"}

The same thing repeated is still illegal:

@a: {osc foo="bar"}
@a: {osc foo="bar"}

The first job of the translator is to figure out which modules and links
would reside inside a FunctionModule of this type.  We do this
before we resolve metalinks, metastrings, etc.

First some preprocessing:
1. we set up the symbol table. All anonymous modules get a name
when they are constructed, so everything has a name.
2. Also, all function inputs, outputs, and attributes should be
removed if they are not matched up with a metalink, etc.,
in the function.
3. create FunctionTranslators for any user defined functions
mentioned anywhere.  Note that the OrchTranslator has pointers
to the functions to be translated.

Then, we create Modules and Links:
1. Then for every module in the symbol table, we create a module
based on the Constructor.  Note that we have to keep the
Constructor around because we will not be able to init everything
until we resolve metalinks and metastrings.
2. then we go through again and add all standard links.

Next, patch up metastrings recursively.

Finally, patch up metalinks:
1. if a function module is constructed,
then any links to it are owned by the function that it is
constructed in, even if those links go several functions
deep through a series of metalinks to metalinks to metalinks
before they actually link to a real module.



*/

class OrchConstructor;
class OrchTranslator;
class OrchFunctionTranslator;
class FunctionModule;
class Module;
class OrchFunction;
class OrchExpression;

class SymbolTableEntry
{
public:
	OrchConstructor *oc;
	Module* mod;
	OrchFunctionTranslator* oft;
};

class AdjacencyListEntry
{
public:
	string src;
	OrchFunctionTranslator* srcoft;
	Link* lnk;
	string dst;
	OrchFunctionTranslator* dstoft;
	string dstattname; // "" if a standard input
};

class OrchFunctionTranslator
{
private:
	// the function we are translating
	OrchFunction *of;
	FunctionModule *fm;
	// having a ref to this lets us look up OrchFunctions
	OrchTranslator *ot;
	map<string, SymbolTableEntry*> symboltable;
	vector<AdjacencyListEntry*> adjacencylist;
	IdentifierType determineIdentifierType(string id);
	map<string, string> metastrings;

	//OrchConstructor findConstructorForName(string name);
	void stageOne(OrchTranslator *ot);
	void stageTwo();
	void setupSymbolTable(OrchExpression *oe);
	void removeExtraParameters();
	void checkForUndefinedReferences();
	void createModules();
	void createFunctionTranslators(OrchTranslator *ot);
	void stateTwo();
	void patchDownMetastrings();
	void patchDownMetalinkAttributes();
	void createLinks(OrchExpression *oe);
	void createAttributeLinks();
	void initSimpleAttributes();
	void findRealDest(AdjacencyListEntry *aje, string inname);
	void findRealDestRecursive(OrchExpression *oe, AdjacencyListEntry *aje, string soughtname);
	void findRealSrc(AdjacencyListEntry *aje, string outname);
	void findRealSrcRecursive(OrchExpression *oe, AdjacencyListEntry *aje, string soughtname);
	void checkAdjacencyList();
	void setMetastring(string, string);
	string getMetastring(string);
	void finalizeModules();
	void finalizeLinks();

	//void setupSymbolTable(OrchExpression*);
	void patchDownMetalinks();

	Module* constructModule(OrchConstructor* oc);
	Module* getModule(string name);

public:
	OrchFunctionTranslator();
	void setFunction(OrchFunction *of);
	OrchFunction *getFunction();

	// this function only called by OrchTranslator
	//void translateEntryPoint(OrchTranslator *ot, string name, Link *note, Link* trig, Link* velo);
	void translateEntryPoint(OrchTranslator *ot, string name);
	void setFunctionModule(FunctionModule *fm);
	FunctionModule *getFunctionModule();
};

