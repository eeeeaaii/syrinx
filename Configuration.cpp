#include "Configuration.h"

Configuration::Configuration()
{
	printDebugOutput = false;
	printModuleDump = false;
	printScoreAST = false;
}

Configuration::~Configuration()
{
}


/* static */
Configuration* Configuration::instance = 0;

/* static */
Configuration* Configuration::getInstance()
{
	if (instance == 0) {
		instance = new Configuration();
	}
	return instance;
}

string Configuration::getUsage()
{
	string s = "Wrong number of arguments.\n";
	s += "Usage: synth SCOREFILE ORCHFILE [OPTIONS]\n";
	s += "   where options are:\n";
	s += "   -d\tprint debug output\n";
	s += "   -dm\tprint module dump\n";
	s += "   -ds\tprint score AST\n";
	s += "   -do\tprint orchestra AST\n";
	return s;
}

bool Configuration::configure(int argc, char** argv)
{
	if (argc < 3) {
		return false;
	}
	scoreFileName = string(argv[1]);
	orchFileName = string(argv[2]);
	for (int i = 3 ; i < argc ; i++) {
		string sarg = string(argv[i]);
		if (sarg == "-d") {
			printDebugOutput = true;
		} else if (sarg == "-dm") {
			printModuleDump = true;
		} else if (sarg == "-ds") {
			printScoreAST = true;
		} else if (sarg == "-do") {
			printOrchAST = true;
		} else {
			return false;
		}
	}
	return true;
}


