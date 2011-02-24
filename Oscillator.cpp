#include "Oscillator.h"
#include "Link.h"

Oscillator::Oscillator()
{
	index = 0;
	increment = 0;
	type = WAVE_SINE;
	level = .2;
	freq = hzToSignal(440.0);
}

string Oscillator::getInfo()
{
	string s;
	s += "Oscillator module";
	return s;
}

void Oscillator::setStringParameter(string pname, string value)
{
	if (pname == string("level")) {
		level = toDouble(value);
	} else if (pname == string("freq")) {
		freq = hzToSignal(toDouble(value));
	} else {
		cout << "invalid parameter.\n";
		exit(1);
	}
}

void Oscillator::deriveIncrement()
{
	double samplespercycle = SRATE / signalToHz(freq);
	increment = (2 * PI) / samplespercycle;
}

void Oscillator::processOneSample()
{
	// for now we are doing a sine wave.
	// we are given sample rate
	// we want index to go from 0 to 2*PI
	// frequency times per srate samples.
	if (outputs.size() > 0) {
		double output;
		if (isLinked("level")) {
			level = getLinkParameter("level");
		}
		if (isLinked("freq")) {
			freq = getLinkParameter("freq");
		}
		deriveIncrement();
		if (type == WAVE_SINE) {
			output = sin(index);
		} else if (type == WAVE_SQUARE) {
			if (index < PI) {
				output = 1.0;
			} else {
				output = -1.0;
			}
		}
		output *= level;
		for (int i = 0 ; i < outputs.size() ; i++) {
			outputs.at(i)->putData(output);
		}
		index += increment;
		if (index >= (2 * PI)) index -= (2 * PI);
	}
}


