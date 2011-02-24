#pragma once
#include "standardincludes.h"

/*
 *
 * This class parses the command line options
 * and stores them for later access.
 * It is a singleton so any code can access command line
 * options.
 *
 */

class Configuration
{
private:
	static Configuration *instance;
	Configuration();
public:
	bool printDebugOutput;
	bool printModuleDump;
	bool printScoreAST;
	bool printOrchAST;
	string orchFileName;
	string scoreFileName;
	static Configuration* getInstance();

	bool configure(int argc, char** argv);
	string getUsage();
	virtual ~Configuration();
};
