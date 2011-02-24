#pragma once
#include <string>

using namespace std;

// utility classes to get around the fact
// that you can't have a vector in %union

// should template out stringlist/oclist

class OrchExpression;

class stringlist
{
public:
	string s;
	stringlist *next;
	stringlist() {
		next = 0;
	}
	~stringlist() {
		if (next) {
			delete next;
		}
	}
};
class pairlist
{
public:
	string first;
	string second;
	pairlist *next;
	pairlist() {
		next = 0;
	}
	~pairlist() {
		if (next) {
			delete next;
		}
	}
};
class oelist
{
public:
	OrchExpression *oe;
	oelist *next;
	oelist() {
		next = 0;
	}
	~oelist() {
		if (next) {
			delete next;
		}
	}
};
