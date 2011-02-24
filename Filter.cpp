#include "Filter.h"
#include "Link.h"

Filter::Filter()
{
	previous = 0.0;
}

void Filter::processOneSample()
{
	if (outputs.at(0)) {
		double input = inputs.at(0)->getData();
		double output = input * .5 + previous * .5;
		previous = output;
		outputs.at(0)->putData(output);
	}
}


string Filter::getInfo()
{
	return string("Filter module");
}
