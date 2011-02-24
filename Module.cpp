#include "Module.h"
#include "Link.h"

Module::Module()
{
}

Module::~Module()
{
}

void Module::addInput(int at, Link* newlink)
{
	if (at <= getMaxInputs()) {
		if (at >= inputs.size()) {
			inputs.resize(inputs.size() * 2, (Link*)0);
		}
		inputs.insert(inputs.begin() + at, newlink);
	}
	newlink->setOutput(this);
}

void Module::addInput(Link* newlink)
{
	if (inputs.size() < getMaxInputs()) {
		inputs.push_back(newlink);
	}
	newlink->setOutput(this);
}

void Module::addOutput(Link* newlink)
{
	if (outputs.size() < getMaxOutputs()) {
		outputs.push_back(newlink);
	}
	newlink->setInput(this);
}

void Module::addOutput(int at, Link* newlink)
{
	if (at <= getMaxOutputs()) {
		if (at >= outputs.size()) {
			outputs.resize(outputs.size() * 2, (Link*)0);
		}
		outputs.insert(outputs.begin() + at, newlink);
	}
	newlink->setInput(this);
}

void Module::processOneSample()
{
}

void Module::processOneControlEvent()
{
}

double Module::getLinkParameter(string pname)
{
	return linkparameters[pname]->getData();
}

void Module::addLinkParameter(string pname, Link* pval)
{
	linkparameters[pname] = pval;
	pval->setOutput(this);
}

bool Module::isLinked(string pname)
{
	return (linkparameters.find(pname) != linkparameters.end());
}

void Module::setStringParameter(string pname, string val)
{
}

string Module::fullDebug()
{
	string s;
	s = "";
	s += "::" + nameDebug() + "::\n";
	for (int i = 0 ; i < inputs.size() ; i++) {
		s += inputs[i]->getInput()->nameDebug() + ">>\n";
	}
	for (int i = 0 ; i < outputs.size() ; i++) {
		s += ">>" + inputs[i]->getOutput()->nameDebug() + "\n";
	}
	return s;
}

void Module::trace()
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
}

string Module::getInfo()
{
	return string("unknown module");
}

void Module::finalize()
{
}
