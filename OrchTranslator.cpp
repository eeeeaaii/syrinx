#include "OrchTranslator.h"
#include "OrchFunction.h"
#include "OrchFunctionTranslator.h"
#include "FunctionModule.h"

OrchTranslator::OrchTranslator()
{
}

/*
 * void addFunction(OrchFunction *)
 *
 * This function is called within orchlang.l.
 * An OrchFunction is a representation of the source
 * code of that function.
 */

void OrchTranslator::addFunction( OrchFunction *of)
{
	definedfunctions[of->getName()] = of;
}

OrchFunction* OrchTranslator::getFunctionFromName(string name)
{
	if (definedfunctions.find(name) != definedfunctions.end()) {
		return definedfunctions[name];
	} else return 0;
}

string OrchTranslator::dump()
{
	string r = "OrchTranslator: {\n";
	map<string, OrchFunction*>::iterator i;
	for (i = definedfunctions.begin() ; i != definedfunctions.end() ; i++) {
		r += (*i).second->dump();
	}
	r += "}\n";
	return r;
}

/*
 * This function accepts a string argument which is the
 * name of the function to translate.  It translates it
 * into a FunctionModule and returns it (or zero if the
 * function is not found)
 */
FunctionModule* OrchTranslator::translate(string name, Link* note, Link* trig, Link* velo)
{
	TRACE("OrchTranslator", "translating");
	OrchFunction *of = getFunctionFromName(name);
	if (!of) return 0;
	OrchFunctionTranslator ofi;
	ofi.setFunction(of);
	//ofi.translateEntryPoint(this, name, note, trig, velo);
	//ofi.addInput("@@note", note);
	//ofi.addInput("@@trig", trig);
	//ofi.addInput("@@velo", velo);
	ofi.translateEntryPoint(this, name);
	TRACE("OrchTranslator", "done getting entry point");
	return ofi.getFunctionModule();
}
