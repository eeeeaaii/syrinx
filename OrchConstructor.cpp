#include "OrchConstructor.h"

OrchConstructor::OrchConstructor()
{
}


void OrchConstructor::addAttribute(string nm, string val)
{
	attributes[nm] = val;
}

void OrchConstructor::setName(string n)
{
	name = n;
}

string OrchConstructor::getName()
{
	return name;
}

map<string, string> OrchConstructor::getAttributes()
{
	return attributes;
}

