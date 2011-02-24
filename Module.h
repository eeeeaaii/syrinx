#pragma once
#include "standardincludes.h"

/*
 * why aren't link parameters just links?
 * I guess because they have names?
 * actually -- the inputs should be a linkmap
 * there should be no linkGroup for inputs.
 */

class Link;
class Module
{
protected:
	linkGroup outputs;
	linkGroup inputs;
	linkMap linkparameters;

	virtual int getMaxOutputs() = 0;
	virtual int getMaxInputs() = 0;
	double getLinkParameter(string pname);
public:
	Module();
	virtual ~Module();
	virtual void processOneSample();
	virtual void processOneControlEvent();
	void addInput(int, Link*);
	void addOutput(int, Link*);
	void addInput(Link*);
	void addOutput(Link*);
	void addLinkParameter(string, Link*);
	bool isLinked(string);
	virtual void setStringParameter(string, string);
	virtual string getInfo();
	virtual void trace();
	virtual void finalize();
	string fullDebug();
	virtual string nameDebug() = 0;
};

