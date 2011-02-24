#pragma once
#include "standardincludes.h"

class FunctionModule;
class OrchConstructor
{
private:
	string name;
	map<string, string> attributes;
	map<string, FunctionModule*> translated_functions;
public:
	OrchConstructor();
	void addAttribute(string, string);
	void setName(string);
	string getName();
	map<string, string> getAttributes();
};
