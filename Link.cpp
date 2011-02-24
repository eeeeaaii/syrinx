#include "Link.h"
#include "Module.h"

int Link::nextid = 0;

Link::Link()
{
	newval = false;
	output = input = 0;
	ostringstream ost;
	ost << "link" << nextid++;
	id = ost.str();
}

Link::Link(double val)
{
	newval = false;
	out = in = val;
	output = input = 0;
	ostringstream ost;
	ost << "link" << nextid++;
	id = ost.str();
}

double Link::getData()
{
	return out;
}

void Link::putData(double _in)
{
	in = _in;
	newval = true;
}

void Link::propagate()
{
	if (newval) {
		TRACE("Link", string("propagating data from ") + input->getInfo() + string(" to ") + output->getInfo());
		out = in;
		newval = false;
	}
}

void Link::setOutput(Module *m)
{
	output = m;
}

void Link::setInput(Module *m)
{
	input = m;
}

Module *Link::getInput()
{
	return input;
}

Module *Link::getOutput()
{
	return output;
}

bool Link::hasOutput()
{
	TRACE("Link", "wambam");
	return (output != 0);
}

bool Link::hasInput()
{
	return (input != 0);
}
