#include "Mixer.h"
#include "Link.h"

void Mixer::processOneSample()
{
	if (outputs.at(0)) {
		double output = 0;
		double data;
		int sz;
		sz = inputs.size();
		for (int i = 0 ; i < sz ; i++) {
			output += inputs.at(i)->getData() / sz;
		}
		outputs.at(0)->putData(output);
	}
}

string Mixer::getInfo()
{
	return string("mixer module.");
}

