#include "Controller.h"
#include "Module.h"
#include "Link.h"
#include "ModuleFactory.h"
#include "Logger.h"
#include "Configuration.h"

Controller globalcontroller;

Controller::Controller()
{
	modulefactory = new ModuleFactory();
	controlcounter = SRATE / CRATE;
}

Controller::~Controller()
{
	delete modulefactory;
}

void Controller::start()
{
	int stopat = 1000;
	int stopcounter = 0;
	for(int c = 0;;c = (c + 1) % controlcounter) {
		if (stopcounter++ == stopat) break;
		if (c == 0) {
			for (modGroupIter i = modules.begin() ; i != modules.end() ; i++) {
				(*i)->processOneControlEvent();
			}
		}
		for (modGroupIter i = modules.begin() ; i != modules.end() ; i++) {
			(*i)->processOneSample();
		}
		for (linkGroupIter i = links.begin() ; i != links.end() ; i++) {
			(*i)->propagate();
		}
	}
}

void Controller::init()
{
	modulefactory->setupModules(modules, links);

	if (Configuration::getInstance()->printModuleDump) {
		for (modGroupIter i = modules.begin() ; i != modules.end() ; i++) {
			cout << (*i)->fullDebug();
			//(*i)->trace();
		}
	}
}

static void handleexit(int signum)
{
	globalcontroller.finalize();
}


void Controller::finalize()
{
	for (modGroupIter i = modules.begin() ; i != modules.end() ; i++) {
		(*i)->finalize();
	}
	exit(0);
}

int main(int argc, char** argv)
{
	// set up exit handler
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = handleexit;
	sigaction(SIGINT, &act, NULL);

	Configuration *config = Configuration::getInstance();
	if (!config->configure(argc, argv)) {
		cout << config->getUsage();
		exit(1);
	}

	//Controller c;
	globalcontroller.init();
	globalcontroller.start();
}

