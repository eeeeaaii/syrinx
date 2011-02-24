#pragma once
#include "standardincludes.h"
#include "Module.h"

// for sine wave
#define PI 3.14159


/*
 * Output 1: the sound
 * Input 1: the pitch to play
 */

class Oscillator : public Module
{
private:
	void deriveIncrement();
protected:
	virtual int getMaxOutputs() { return 1;}
	virtual int getMaxInputs() { return 0; }
	virtual string nameDebug() { return string("oscillator"); }
	double freq;
	double index;
	double increment;
	double level;
	wavetype type;
public:
	Oscillator::Oscillator();
	virtual void processOneSample();
	virtual void setStringParameter(string pname, string value);
	virtual string getInfo();
};

