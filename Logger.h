#include "standardincludes.h"

class Logger
{
public:
	class LogEntry;
	static void log(string where);
	static void dumplog();
private:
	static vector<LogEntry> thelog;
};


class Logger::LogEntry
{
public:
	LogEntry(string wr, struct timeval ut);
	string where;
	struct timeval utime;
};
