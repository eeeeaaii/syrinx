#pragma once
#include "standardincludes.h"

static void handleexit(int signum);

class Module;
class Link;
class ModuleFactory;
class Controller
{
private:
	modGroup modules; // this will be top-level function modules and sequencers only
	linkGroup links; // links from function modules to sequencers, etc.
	
	ModuleFactory *modulefactory;
	int controlcounter;
public:
	Controller();
	virtual ~Controller();
	void start(void);
	void init();
	void finalize();
};
