#pragma once
#include "standardincludes.h"
#include "Module.h"

/*
 * Output 1: the sound
 * Input 1: the sound
 */

class Mixer : public Module
{
protected:
	virtual int getMaxOutputs() { return 1;}
	virtual int getMaxInputs() { return 999; }
	virtual string nameDebug() { return string("mixer"); }
public:
	virtual void processOneSample();
	virtual string getInfo();
};

