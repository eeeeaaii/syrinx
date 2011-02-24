#pragma once
#include "standardincludes.h"
#include "Module.h"

/*
 * Output 1: the sound
 * Input 1: the sound
 */

class Filter : public Module
{
protected:
	virtual int getMaxOutputs() { return 1;}
	virtual int getMaxInputs() { return 1; }
	double previous;
public:
	Filter::Filter();
	virtual void processOneSample();
	virtual string getInfo();
	virtual string nameDebug() { return string("filter"); }
};

