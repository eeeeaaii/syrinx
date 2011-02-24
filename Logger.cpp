#include "Logger.h"

vector<Logger::LogEntry> Logger::thelog;

void Logger::log(string where)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	thelog.push_back(LogEntry(where, t));
}

Logger::LogEntry::LogEntry(string _where, struct timeval _ut)
{
	where = _where;
	utime = _ut;
}

void Logger::dumplog()
{
	vector<LogEntry>::iterator i;
	FILE *f = fopen("synth.log", "wa");
	long first = 0;
	for (i = thelog.begin() ; i != thelog.end() ; i++) {
		//cout << (*i).where << " " << (*i).when << "\n";
		if (i == thelog.begin()) {
			first = (*i).utime.tv_usec;
		}
		fprintf(f, "%s %d %d\n", (*i).where.c_str(), (*i).utime.tv_usec - first);
	}
	fclose(f);
}


