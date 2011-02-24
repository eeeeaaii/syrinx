#include "OrchFunctionTranslator.h"
#include "OrchConstructor.h"
#include "OrchExpression.h"
#include "OrchFunction.h"
#include "OrchTranslator.h"
#include "Module.h"
#include "Oscillator.h"
#include "Filter.h"
#include "Output.h"
#include "Mixer.h"
#include "FileOut.h"
#include "FunctionModule.h"
#include "Link.h"

/*
 * note, velo, etc. should not be args to translateEntryPoint.
 * instead we call setInput("@@note", n)
 * we remove the recursive calls, instead we set and get inputs.
 */

/*
 * What I'm doing is crazy.  I need to change the FunctionModule
 * class so that is has a vector of inputs and a vector
 * of outputs.  Then I can set them, retrieve them, whatever.
 */

/*
refresher:

> @mod1 / @mymod
mod1's output hooked to mymod's input

@mymod \ @mod2 <
mymod's output hooked to mod2's input

> @mod1 / @mymod \ @mod2 <
both

@mymod: { func a="b" }
define @mymod as a func module with a equal to value b.

> { f2 a="b" } / @mymod
create an anonymous f2 module and send the output into
the input of mymod.

myfunc | @@out
[[
	@x: {f1 a="b"}
	@y: {f1 a="b"}
	@x / @y / @@out < <
]]
define a function called myfunc, which has an output
of @@out





 */


/*
 * TO DO: figure out why @@note never makes it into adjacency list.
 *
 */

OrchFunctionTranslator::OrchFunctionTranslator()
{
}


// this function is only called by OrchTranslator
// I go through stageOne, then recursively call stageOne
// on all the FunctionTranslators pointed to in the symboltable.
// then I go through stageTwo, and call stageTwo on
// all the FunctionTranslators in the symboltable.
void OrchFunctionTranslator::translateEntryPoint(OrchTranslator *ot, string fname)
{
	TRACE("OrchFunctionTranslator", string("translate entry point for ") + fname);

	fm = new FunctionModule(fname);
	stageOne(ot);
	stageTwo();
	finalizeModules();
	finalizeLinks();
	TRACE("OrchFunctionTranslator", string("done translation for ") + fname);
}

/*
 * stage one is everything up to the creation of the FunctionTranslators
 *
 */
void OrchFunctionTranslator::stageOne(OrchTranslator *ot)
{
	TRACE("OrchFunctionTranslator", string("stage 1"));
	SymbolTableEntry *ste;
	vector<OrchExpression*> expr;
	

	expr = of->getExpressions();
	for (vector<OrchExpression*>::iterator i = expr.begin() ; i != expr.end() ; i++) {
		setupSymbolTable(*i);
	}
	removeExtraParameters();
	checkForUndefinedReferences();
	createModules();
	createFunctionTranslators(ot);
	for (map<string, SymbolTableEntry*>::iterator j = symboltable.begin() ; j != symboltable.end() ; j++) {
		ste = j->second;
		if (ste->oft) {
			ste->oft->stageOne(ot);
		}
	}
}

// this is recursive: adds a symbol for the expression
// passed to it, then recurses on input and output list.
void OrchFunctionTranslator::setupSymbolTable(OrchExpression *e)
{
	string nm = e->getName();
	TRACE("OrchFunctionTranslator", string("setting up symbol table for ") + nm);
	// will return 0 if no constructor -- 0 is okay, not an error
	OrchConstructor* oc = e->getConstructor();
	//cout << "oc: " << oc;
	// look to see if this symbol is already in table
	if (symboltable.find(nm) == symboltable.end()) {
		// if not, add it
		SymbolTableEntry *ste = new SymbolTableEntry();
		// if no constructor, desired result is that ste->constructor == 0
		// so we can do a straight assignment
		ste->oc = oc;
		symboltable[nm] = ste;
	} else {
		// ok, it is already in table
		if (symboltable[nm]->oc == 0) {
			// no constructor yet
			symboltable[nm]->oc = oc;
		} else if (oc) {
			// trying to assign another constructor to same name
			// error!
			cout << "trying to assign another constructor to " + nm;
			assert(false);
		}
	}
	// now go through inputs and outputs
	vector<OrchExpression*>::iterator i;

	vector<OrchExpression*> inputs;
	inputs = e->getInputs();
	for (i = inputs.begin() ; i != inputs.end() ; i++) {
		setupSymbolTable(*i);
	}

	vector<OrchExpression*> outputs;
	outputs = e->getOutputs();
	for (i = outputs.begin() ; i != outputs.end() ; i++) {
		setupSymbolTable(*i);
	}
}

void OrchFunctionTranslator::removeExtraParameters()
{
}

/*
 * At this point everything in the symboltable should have
 * a module definition
 */
void OrchFunctionTranslator::checkForUndefinedReferences()
{
	map<string, SymbolTableEntry*>::iterator i;
	for (i = symboltable.begin() ; i != symboltable.end() ; i++) {
		if (i->second->oc == 0) {
			assert(false);
		}
	}
}

/*
 * create a module for each symboltable entry
 */
void OrchFunctionTranslator::createModules()
{
	SymbolTableEntry *ste;
	map<string, SymbolTableEntry*>::iterator i;
	for (i = symboltable.begin() ; i != symboltable.end() ; i++) {
		ste = i->second;
		ste->mod = constructModule(ste->oc);
		// don't init args here -- see comment in 
		// getMetastrings
		//sdflksdfinitSimpleAttributes(ste->oc, ste->mod);
		//
	}
}

/*
 * for each symbol table entry that is a FunctionModule,
 * create a FunctionTranslator
 *
 */
void OrchFunctionTranslator::createFunctionTranslators(OrchTranslator *ot)
{
	SymbolTableEntry *ste;
	OrchFunction *of;
	map<string, SymbolTableEntry*>::iterator i;
	for (i = symboltable.begin() ; i != symboltable.end() ; i++) {
		ste = i->second;
		Module* m = ste->mod;
		if (typeid(*m) == typeid(FunctionModule)) {
			of = ot->getFunctionFromName(i->first);
			if (!of) {
				// undefined function!
				assert(false);
			}
			ste->oft = new OrchFunctionTranslator();
			ste->oft->setFunction(of);
		} else {
			ste->oft = 0;
		}
	}
}

//----------------------------


Module *OrchFunctionTranslator::constructModule(OrchConstructor *oc)
{
	Module *m;
	string name = oc->getName();
	if (name == "osc") {
		m = new Oscillator();
	} else if (name == "filt") {
		m = new Filter();
	} else if (name == "out") {
		m = new Output();
	} else if (name == "mix") {
		m = new Mixer();
	} else if (name == "fileout") {
		m = new FileOut();
	} else {
		m = new FunctionModule(name);
	}
	return m;
}

//----------------------------

void OrchFunctionTranslator::stageTwo()
{
	TRACE("OrchFunctionTranslator", string("stage 2"));
	SymbolTableEntry *ste;
	patchDownMetastrings();
	patchDownMetalinkAttributes();
	vector<OrchExpression*>::iterator i;
	vector<OrchExpression*> expr;
	expr = of->getExpressions();
	for (i = expr.begin() ; i != expr.end() ; i++) {
		createLinks(*i);
	}
	createAttributeLinks();
	initSimpleAttributes();
	checkAdjacencyList();
	for (map<string, SymbolTableEntry*>::iterator j = symboltable.begin() ; j != symboltable.end() ; j++) {
		ste = j->second;
		if (ste->oft) {
			ste->oft->stageTwo();
		}
	}
}

void OrchFunctionTranslator::patchDownMetastrings()
{
	map<string, SymbolTableEntry*>::iterator i;
	SymbolTableEntry *ste;
	// go through the symbol table
	for (i = symboltable.begin() ; i != symboltable.end() ; i++) {
		ste = i->second;
		assert(ste);
		// get the OFT 
		OrchFunctionTranslator *fte = ste->oft;
		// if OFT is 0, it means this is
		// not a FunctionModule, it's just a regular module.
		if (fte == 0) continue;
		// get the source code for this function
		OrchFunction *of = fte->getFunction();
		assert(of);
		OrchConstructor *oc = ste->oc;
		map<string, string> attributes;
		attributes = oc->getAttributes();
		map<string, string>::iterator j;
		// loop through the attributes that were passed to the function when
		// it was invoked (i.e. via the constructor)
		for ( j = attributes.begin() ; j != attributes.end() ; j++) {
			string attname = j->first;
			string attval = j->second;
			// every attribute should have an attribute param
			// if it doesn't there's something wrong.
			string paramval = of->getAttributeParam(attname);
			assert(paramval != ""); // error! unknown attribute
			// in this function, we only do something with the metastrings
			if (determineIdentifierType(paramval) == METASTRING) {
				// we need to find out the type of the thing that's
				// being fed into the function
				IdentifierType attvaltype;
				attvaltype = determineIdentifierType(attval);
				if (attvaltype == STRING) {
					// i.e. att="value"
					fte->setMetastring(paramval, attval);
				} else if (attvaltype == METASTRING) {
					// i.e. att=#value
					fte->setMetastring(paramval, getMetastring(attval));
				} else {
					// i.e. att=@value or att=@@value
					// type mismatch!  trying to init a metastring with a metalink or link!
					assert(false);
				}
			}
		}
	}
}


/*
	here's where metalink attributes get treated:
	{ osc pitch=@@something }
	@@note should only work in the top level function.
	- metalink attributes are always outputs from something
	  going into a module
	- it could be an output from a function module, in which
	  case the actual module is deep down in there.
	- 
*/

void OrchFunctionTranslator::patchDownMetalinkAttributes()
{
}


/*
 * This needs an explanation as to why metalinks should already
 * be resolved at this point.
 * This does not seem to apply to @@note.  Not sure about
 * metalinks at attributes (see createAttributeLinks)
 *
 * The reason you don't have to test for metalinks is that
 * findRealSrc, findRealDest, findRealSrcRecursive, and
 * findRealDestRecursive all find them first.
 *
 */

void OrchFunctionTranslator::createLinks(OrchExpression *e)
{
	string nm = e->getName();
	SymbolTableEntry *ste = symboltable[nm];
	// if oft != 0, it's a function
	OrchFunctionTranslator *oft = ste->oft;

	// go through inputs and outputs
	// and set up adjacency list entries
	//
	// some of these will be metalinks.
	vector<OrchExpression*>::iterator i;

	vector<OrchExpression*> inputs;
	vector<OrchExpression*> outputs;
	inputs = e->getInputs();
	outputs = e->getOutputs();

	for (i = inputs.begin() ; i != inputs.end() ; i++) {
		string inname = (*i)->getName();
		if (determineIdentifierType(inname) == METALINK) {
			// ignore, has already been resolved
		} else {
			AdjacencyListEntry *aje = new AdjacencyListEntry();
			aje->src = (*i)->getName();
			aje->srcoft = 0;
			aje->dst = string("");
			aje->dstoft = 0;
			if (oft != 0) {
				oft->findRealDest(aje, inname);
			} else {
				aje->dst = nm;
				aje->dstoft = 0;
			}
			adjacencylist.push_back(aje);
		}
	}
	for (i = outputs.begin() ; i != outputs.end() ; i++) {
		string outname = (*i)->getName();
		if (determineIdentifierType(outname) == METALINK) {
			// ignore, has already been resolved
		} else {
			AdjacencyListEntry *aje = new AdjacencyListEntry();
			aje->src = string("");
			aje->srcoft = 0;
			if (oft != 0) {
				oft->findRealSrc(aje, outname);
			} else {
				aje->src = nm;
				aje->srcoft = 0;
			}
			aje->dst = (*i)->getName();
			aje->dstoft = 0;
			adjacencylist.push_back(aje);
		}
	}
	// then do it recursively
	for (i = inputs.begin() ; i != inputs.end() ; i++) {
		createLinks(*i);
	}
	for (i = outputs.begin() ; i != outputs.end() ; i++) {
		createLinks(*i);
	}

}

/*

Loop through the symbol table and, for each module,
loop through its attributes looking for a link input, e.g.:
{ osc pitch=@in1 }
If found, add entry to adjacency list.

*/
void OrchFunctionTranslator::createAttributeLinks()
{
	map<string, SymbolTableEntry*>::iterator i;
	SymbolTableEntry *ste;
	for (i = symboltable.begin() ; i != symboltable.end() ; i++) {
		ste = i->second;
		assert(ste);
		OrchConstructor *oc = ste->oc;
		assert(oc);
		map<string, string> attributes;
		attributes = oc->getAttributes();
		map<string, string>::iterator j;
		for (j = attributes.begin() ; j != attributes.end() ; j++) {
			if (determineIdentifierType(j->second) == LINK) {
				AdjacencyListEntry *aje = new AdjacencyListEntry();
				if (symboltable.find(j->second) == symboltable.end()) {
					// note we have to check to make sure it exists
					// with pipelined inputs and outputs you have
					// already run a check for undefined references
					// in the symbol table -- but doesn't help
					// find undefined attribute=@foo's
					assert(false); // no such thing
				}
				aje->src = j->second;
				aje->srcoft = 0;
				aje->dst = i->first;
				aje->dstoft = 0;
				aje->dstattname = j->first;
				adjacencylist.push_back(aje);
			}
		}
	}
}

void OrchFunctionTranslator::initSimpleAttributes()
{
	map<string, SymbolTableEntry*>::iterator i;
	SymbolTableEntry *ste;
	OrchConstructor *oc = ste->oc;
	Module *m = ste->mod;
	for (i = symboltable.begin() ; i != symboltable.end() ; i++) {
		map<string, string> atts;
		atts = oc->getAttributes();
		map<string, string>::iterator i;
		//loop thru atts and init anything normal
		for (i = atts.begin() ; i != atts.end() ; i++) {
			// check to see whether valid parameter?
			if (determineIdentifierType(i->second) == METASTRING) {
				m->setStringParameter(i->first, i->second);
			}
		}
	}
}

/*
 * I loop through the attributes in the constructor:

{ myfunc foo="bar" x="y" }

and for each one, I retrieve the attributeparam from 


 */
void OrchFunctionTranslator::setFunction(OrchFunction *new_of)
{
	of = new_of;
}

OrchFunction *OrchFunctionTranslator::getFunction()
{
	return of;
}

FunctionModule *OrchFunctionTranslator::getFunctionModule()
{
	return fm;
}

// this means that an input is going into a function module, and we want to search down
// through that function and its child functions until we find the module
// that this input actually links to.
void OrchFunctionTranslator::findRealDest(AdjacencyListEntry *aje, string inname)
{
	// loop thru the expressions and 
	vector<OrchExpression*>::iterator i;
	vector<OrchExpression*> expr;
	expr = of->getExpressions();
	for (i = expr.begin() ; i != expr.end() ; i++) {
		findRealDestRecursive(*i, aje, inname);
	}
}

void OrchFunctionTranslator::findRealDestRecursive(OrchExpression *oe, AdjacencyListEntry *aje, string soughtname)
{
	string nm = oe->getName();
	SymbolTableEntry *ste = symboltable[nm];
	// if oft != 0, it's a function
	OrchFunctionTranslator *oft = ste->oft;

	vector<OrchExpression*>::iterator i;
	vector<OrchExpression*> inputs;
	vector<OrchExpression*> outputs;
	inputs = oe->getInputs();
	outputs = oe->getOutputs();

	for (i = inputs.begin() ; i != inputs.end() ; i++) {
		string inname = (*i)->getName();
		if (inname == soughtname) {
			if (aje->dst != string("")) {
				// error -- cannot be hooked into more than one destination
				assert(false);
			}
			if (oft != 0) {
				aje->dst = nm;
				aje->dstoft = this;
				return;
			} else {
				oft->findRealDest(aje, soughtname);
			}
		}
	}
	for (i = outputs.begin() ; i != outputs.end() ; i++) {
		string outname = (*i)->getName();
		if (outname == soughtname) {
			// error!  cannot hook an output up to an output!
			assert(false);
		}
	}
	// then do it recursively
	for (i = inputs.begin() ; i != inputs.end() ; i++) {
		findRealDestRecursive(*i, aje, soughtname);
	}
	// yes, I do the outputs, because some of the outputs could have
	// their own inputs (theoretically).
	for (i = outputs.begin() ; i != outputs.end() ; i++) {
		findRealDestRecursive(*i, aje, soughtname);
	}
}

// this means that an output is coming from a function module, and we need to
// search down through that function and its child functions until we
// find the module that this link is actually coming from.
void OrchFunctionTranslator::findRealSrc(AdjacencyListEntry *aje, string outname)
{
	// loop thru the expressions and 
	vector<OrchExpression*>::iterator i;
	vector<OrchExpression*> expr;
	expr = of->getExpressions();
	for (i = expr.begin() ; i != expr.end() ; i++) {
		findRealSrcRecursive(*i, aje, outname);
	}
}

void OrchFunctionTranslator::findRealSrcRecursive(OrchExpression *oe, AdjacencyListEntry *aje, string soughtname)
{
	string nm = oe->getName();
	SymbolTableEntry *ste = symboltable[nm];
	// if oft != 0, it's a function
	OrchFunctionTranslator *oft = ste->oft;

	vector<OrchExpression*>::iterator i;
	vector<OrchExpression*> inputs;
	vector<OrchExpression*> outputs;
	inputs = oe->getInputs();
	outputs = oe->getOutputs();

	for (i = inputs.begin() ; i != inputs.end() ; i++) {
		string inname = (*i)->getName();
		if (inname == soughtname) {
			// error!  cannot hook a source up to a source
			assert(false);
		}
	}
	for (i = outputs.begin() ; i != outputs.end() ; i++) {
		string outname = (*i)->getName();
		if (outname == soughtname) {
			if (aje->src != string("")) {
				// error -- cannot be hooked into more than one destination
				assert(false);
			}
			if (oft != 0) {
				aje->src = nm;
				aje->srcoft = this;
				return;
			} else {
				oft->findRealSrc(aje, soughtname);
			}
		}
	}
	// then do it recursively
	for (i = inputs.begin() ; i != inputs.end() ; i++) {
		findRealSrcRecursive(*i, aje, soughtname);
	}
	// yes, I do the outputs, because some of the outputs could have
	// their own inputs (theoretically).
	for (i = outputs.begin() ; i != outputs.end() ; i++) {
		findRealSrcRecursive(*i, aje, soughtname);
	}
}

void OrchFunctionTranslator::checkAdjacencyList()
{
}

void OrchFunctionTranslator::finalizeModules()
{
	TRACE("OrchFunctionTranslator", string("finalizeModules"));
	map<string, SymbolTableEntry*>::iterator i;
	SymbolTableEntry *ste;
	for (i = symboltable.begin() ; i != symboltable.end() ; i++) {
		ste = i->second;
		fm->addModule(ste->mod);
		if (ste->oft) {
			ste->oft->finalizeModules();
		}
	}
}

/*
 * In the AJE:
 * src is a string indicating the name of the thing the sound comes from
 * srcoft is zero if that module is in this same function,
 * otherwise it is the OFT of the function that has that module.
 * dst and dstoft are the analogs for the place where the sound is
 * going to
 * finally, if the sound is going to an attribute input,
 * dstattname is not a null string.
 */

void OrchFunctionTranslator::finalizeLinks()
{
	TRACE("OrchFunctionTranslator", string("finalizeLinks"));
	for (vector<AdjacencyListEntry*>::iterator i = adjacencylist.begin() ; i != adjacencylist.end() ; i++) {
		Module* srcmod;
		Module* dstmod;
		AdjacencyListEntry *aje;
		aje = *i;
		TRACE("oft", string("src: ") + aje->src);
		TRACE("oft", string("dst:") + aje->dst);
		/*
		if (aje->src == "@@note") {
			TRACE("oft", "@@note");
			aje->lnk = note;
		} else if (aje->src == "@@trig") {
			TRACE("oft", "@@trig");
			aje->lnk = trig;
		} else if (aje->src == "@@velo") {
			TRACE("oft", "@@velo");
			aje->lnk = velo;
		} else */if (aje->src != "") {
			if (aje->srcoft) {
				srcmod = aje->srcoft->getModule(aje->src);
			} else {
				srcmod = getModule(aje->src);
			}
			aje->lnk = new Link();
		}
		if (aje->dstoft) {
			dstmod = aje->dstoft->getModule(aje->dst);
		} else {
			dstmod = getModule(aje->dst);
		}
		srcmod->addOutput(aje->lnk);
		if (aje->dstattname != "") {
			dstmod->addLinkParameter(aje->dstattname, aje->lnk);
		} else {
			dstmod->addInput(aje->lnk);
		}

	}
}

void OrchFunctionTranslator::setMetastring(string name, string value)
{
	metastrings[name] = value;
}

string OrchFunctionTranslator::getMetastring(string name)
{
	return metastrings[name];
}

IdentifierType OrchFunctionTranslator::determineIdentifierType(string id)
{
	/*
	LINK,  // @whatever   		@a
	STRING, // "whatever"  		""
	METALINK,  // @@whatever  	@@a
	METASTRING,  // #whatever 	#a
	ANONLINK  // %%whatever  	%%a
	*/
	assert(id.size() >= 2);
	if (id[0] == '\"') return STRING;
	if (id[0] == '#') return METASTRING;
	if (id[0] == '@' && id[1] == '@') return METALINK;
	if (id[0] == '@') return LINK;
	if (id[0] == '%' && id[1] == '%') return ANONLINK;
	assert(false);
}

Module* OrchFunctionTranslator::getModule(string n)
{
	return symboltable[n]->mod;
}
