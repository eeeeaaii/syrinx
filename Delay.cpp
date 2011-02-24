#include "Delay.h"
#include "Link.h"

Delay::Delay()
{
	delaytime = .3;
	feedback = .8;
	current = 0;
	setBuffer();
}

void Delay::setBuffer()
{
	int newsize = (int)(delaytime * SRATE);
	buffer = vdouble(newsize, 0.0);
}

void Delay::processOneSample()
{
	if (outputs.at(0)) {
		double input = inputs.at(0)->getData();
		buffer[current] = input + buffer[current] * feedback;
		outputs.at(0)->putData(buffer[current]);
		current = (current + 1) % buffer.size();
	}
}

string Delay::getInfo()
{
	return string("Delay module");
}

