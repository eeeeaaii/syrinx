#pragma once
#include "standardincludes.h"
#include "SeqEvent.h"

class Sequence
{
private:
	double tickspersecond;
	vector<vector<SeqEvent> > events;
	vector<vector<SeqEvent> > getEvents();

public:
	Sequence();
	Sequence(const Sequence& rhs);
	Sequence& operator=(const Sequence& rhs);
	Sequence combineWith(Sequence s);
	void insertSequenceAfter(int position, Sequence s);
	void startNote(int position, SeqEvent s);
	void extendPreviousNote(int position, int notenum, string instrument);
	int getTicksPerSecond();
	int getLength();
	set<string> getInstrumentsUsed();
	void removeAllButOneInstrument(string iname);
};

