#pragma once
#include "standardincludes.h"


class OrchExpression;
class OrchFunction {
private:
	string name;
	vector<OrchExpression*> expressions;
	vector<string> input_params;
	vector<string> output_params;
	map<string, string> attribute_params;
public:
	OrchFunction();
	void setName(string);
	void addInputParam(string);
	void addOutputParam(string);
	void addAttributeParam(string, string);
	void addExpression(OrchExpression*);

	string getName();
	string dump();
	string getAttributeParam(string);
	vector<OrchExpression*> getExpressions();
};

