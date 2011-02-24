#pragma once
#include "standardincludes.h"
#include "Module.h"

/*
 * Output 1: the sound
 * Input 1: the sound
 */

class Delay : public Module
{
private:
	void setBuffer();
protected:
	virtual int getMaxOutputs() { return 1;}
	virtual int getMaxInputs() { return 1; }
	double delaytime; // seconds
	double feedback; // 0.0 - 1.0, percentage
	int current;
	vdouble buffer;
public:
	Delay::Delay();
	virtual void processOneSample();
	virtual string getInfo();
	virtual string nameDebug() { return string("delay"); }
};

