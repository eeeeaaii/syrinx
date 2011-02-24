#include "OrchExpression.h"
#include "OrchConstructor.h"


OrchExpression::OrchExpression()
{
	oc = 0;
}

void OrchExpression::addInput(OrchExpression *in)
{
	inputs.push_back(in);
}

void OrchExpression::addOutput(OrchExpression *out)
{
	outputs.push_back(out);
}

vector<OrchExpression*> OrchExpression::getInputs()
{
	return inputs;
}

vector<OrchExpression*> OrchExpression::getOutputs()
{
	return outputs;
}

void OrchExpression::setName(string nm)
{
	name = nm;
}

string OrchExpression::getName()
{
	return name;
}

void OrchExpression::setConstructor(OrchConstructor* in)
{
	oc = in;
}

OrchConstructor* OrchExpression::getConstructor()
{
	return oc;
}

string OrchExpression::dump()
{
	string r;
	r += "OrchExpression\n";
	return r;
}

//------------------------------------

string OrchExpressionComplete::dump()
{
	string r;
	r += "OrchExpressionComplete\n";
	return r;
}

int OrchExpressionAnonymous::nextID = 0;

OrchExpressionAnonymous::OrchExpressionAnonymous()
{
	char num[10];
	sprintf(num, "%d", nextID);
	name = string("%%anon") + string(num);
	nextID++;
}

string OrchExpressionAnonymous::dump()
{
	string r;
	r += "OrchExpressionAnonymous\n";
	return r;
}

void OrchExpressionAnonymous::setName(string nm)
{
	assert(false);
}

string OrchExpressionReference::dump()
{
	string r;
	r += "OrchExpressionReference\n";
	return r;
}

void OrchExpressionReference::setConstructor(OrchConstructor *in)
{
	assert(false);
}
