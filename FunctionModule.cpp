#include "FunctionModule.h"
#include "Link.h"
	

/*
For this not to be confusing, the links 
between the inner stuff and the 
outer stuff need to be propagated by
the outer stuff
*/

FunctionModule::FunctionModule(string _name)
{
	name = _name;
	input_counter = 0;
	output_counter = 0;
}

void FunctionModule::processOneSample()
{
	for (int i = 0 ; i < inner_modules.size() ; i++) {
		inner_modules.at(i)->processOneSample();
	}
	for (int i = 0 ; i < inner_links.size() ; i++) {
		inner_links.at(i)->propagate();
	}
}
	
Module* FunctionModule::getNextInnerInput()
{
	return inner_inputs[input_counter++];
}

Module* FunctionModule::getNextInnerOutput()
{
	return inner_outputs[output_counter++];
}

void FunctionModule::setNthInnerInput(int n, Module* m)
{
	inner_inputs[n] = m;
}

void FunctionModule::setNthInnerOutput(int n, Module* m)
{
	inner_outputs[n] = m;
}

void FunctionModule::setInnerLinkedAttribute(string name, string modname, Module *m)
{
	paraminit* pi = new paraminit();
	pi->name = modname;
	pi->mod = m;
	inner_linked_attributes[name] = pi;
}
void FunctionModule::initInnerLinkedAttribute(string name, Link* l)
{
	paraminit *pi = inner_linked_attributes[name];
	Module* m = pi->mod;
	m->addLinkParameter(pi->name, l);
	delete pi;
}

string FunctionModule::getInfo()
{
	return string("Function module.");
}

void FunctionModule::trace()
{
	TRACE("modprint", string("this mod:") + getInfo());
	TRACE("modprint", "outputs:");
	for (int i = 0 ; i < outputs.size() ; i++) {
		TRACE("modprint", string("links to ") + outputs[i]->id);
	}
	TRACE("modprint", "inputs:");
	for (int i = 0 ; i < inputs.size() ; i++) {
		TRACE("modprint", string("links to ") + inputs[i]->id);
	}
	TRACE("modprint", string("tracing FunctionModule components"));
	for (int i = 0 ; i < inner_modules.size() ; i++) {
		inner_modules.at(i)->trace();
	}
}

void FunctionModule::addModule(Module *m)
{
	inner_modules.push_back(m);
}

void FunctionModule::addLink(Link *l)
{
	inner_links.push_back(l);
}

void FunctionModule::finalize()
{
	for (int i = 0 ; i < inner_modules.size() ; i++) {
		inner_modules.at(i)->finalize();
	}
}
