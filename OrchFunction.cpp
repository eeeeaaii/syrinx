#include "OrchFunction.h"
#include "OrchExpression.h"

OrchFunction::OrchFunction()
{
}

void OrchFunction::setName(string n)
{
	name = n;
}

string OrchFunction::dump()
{
	string r;
	r += "OrchFunction {\n";
	{
		r += "  inparams: {\n";
		vector<string>::iterator i;
		for (i = input_params.begin() ; i != input_params.end() ; i++) {
			r += "  ";
			r += (*i);
			r += "\n";
		}
		r += "  }\n";
	} {
		r += "  outparams: {\n";
		vector<string>::iterator i;
		for (i = output_params.begin() ; i != output_params.end() ; i++) {
			r += "  ";
			r += (*i);
			r += "\n";
		}
		r += "  }\n";
	} {
		r += "  attributeparams: {\n";
		map<string, string>::iterator i;
		for (i = attribute_params.begin() ; i != attribute_params.end() ; i++) {
			r += "  ";
			r += (*i).first;
			r += " -> ";
			r += (*i).second;
			r += "\n";
		}
		r += "  }\n";
	} {
		r += "expressions: {\n";
		vector<OrchExpression*>::iterator i;
		for (i = expressions.begin() ; i != expressions.end() ; i++) {
			r += (*i)->dump();
		}
		r += "}\n";
	}
	r += "}\n";
	return r;
}

void OrchFunction::addInputParam(string in)
{
	input_params.push_back(in);
}

void OrchFunction::addOutputParam(string in)
{
	output_params.push_back(in);
}

void OrchFunction::addAttributeParam(string nm, string val)
{
	attribute_params[nm] = val;
}

void OrchFunction::addExpression(OrchExpression* e)
{
	expressions.push_back(e);
}

string OrchFunction::getName()
{
	return name;
}

string OrchFunction::getAttributeParam(string nm)
{
	// find whether it is in the map.
	// if it is not in the map, and you use the braces to look for it,
	// you will insert a new value into the map, even if the braces
	// are used in an rval.
	if (attribute_params.find(nm) != attribute_params.end()) {
		return attribute_params[nm];
	} else {
		return string("");
	}
}

vector<OrchExpression*> OrchFunction::getExpressions()
{
	return expressions;
}

