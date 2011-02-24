#include "standardincludes.h"
#include "Configuration.h"


double hzToSignal(double f)
{
	f /= 25000.0;
	f *= 2.0;
	f -= 1.0;
	return f;
}

double signalToHz(double freq)
{
	freq += 1.0;
	freq /= 2.0;
	freq *= 25000.0;
	return freq;
}

double toDouble(string s)
{
	istringstream is(s);
	double d;
	is >> d;
	return d;
}


string selectedsection = "";

void debug(string section, string s)
{
	if (!Configuration::getInstance()->printDebugOutput) return;
	//string selectedsection = "";

	if (selectedsection == "") {
		cout << section << "::" << s << "\n";
	} else if (section == selectedsection) {
		cout << s << "\n";
	}
	cout << flush;
}

void debug(string section, int i)
{
	if (!Configuration::getInstance()->printDebugOutput) return;
	//string selectedsection = "";

	if (selectedsection == "") {
		cout << section << "::" << i << "\n";
	} else if (section == selectedsection) {
		cout << i << "\n";
	}
	cout << flush;
}


