#pragma once
#include "standardincludes.h"


class OrchConstructor;

class OrchExpression
{
private:
	vector<OrchExpression*> inputs;
	vector<OrchExpression*> outputs;
	OrchConstructor *oc;
protected:
	string name;
public:
	OrchExpression();
	virtual void setName(string);
	virtual string dump();
	string getName();
	virtual void setConstructor(OrchConstructor*);
	OrchConstructor* getConstructor();
	void addInput(OrchExpression*);
	void addOutput(OrchExpression*);
	vector<OrchExpression*> getInputs();
	vector<OrchExpression*> getOutputs();
// used in translation?
//	virtual Module* getModule();
};


class OrchExpressionComplete : public OrchExpression
{
public:
	virtual string dump();
};

class OrchExpressionReference : public OrchExpression
{
public:
	void setConstructor(OrchConstructor*);
	virtual string dump();
};

class OrchExpressionAnonymous : public OrchExpression
{
private:
	static int nextID;
public:
	OrchExpressionAnonymous();
	void setName(string);
	virtual string dump();
};

