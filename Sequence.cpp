#include "Sequence.h"
#include "SeqEvent.h"

Sequence::Sequence()
{
}

Sequence::Sequence(const Sequence& rhs)
{
	//events = (const vector<vector<SeqEvent se> >)rhs.events;
	events = rhs.events;
	tickspersecond = rhs.tickspersecond;
}

Sequence& Sequence::operator=(const Sequence& rhs)
{
	events = rhs.events;
	tickspersecond = rhs.tickspersecond;
}

Sequence Sequence::combineWith(Sequence s)
{

	vector<vector<SeqEvent> > e;
	e = s.getEvents();
	for (int i = 0 ; i < e.size(); i++) {
		vector<SeqEvent>::iterator it;
		for (it = e[i].begin() ; it != e[i].end() ; it++) {
			events[i].push_back(*it);
		}
	}
}

set<string> Sequence::getInstrumentsUsed()
{
	set<string> instr;
	for (int i = 0 ; i < events.size() ; i++) {
		vector<SeqEvent>::iterator it;
		for (it = events[i].begin() ; it != events[i].end() ; it++) {
			instr.insert(it->getInstrument());
		}
	}
	return instr;
}

void Sequence::insertSequenceAfter(int position, Sequence s)
{
	vector<vector<SeqEvent> > e;
	vector<vector<SeqEvent> >::iterator i;
	i = i + position;
	e = s.getEvents();
	events.insert(i, e.begin(), e.end());
}

vector<vector<SeqEvent> > Sequence::getEvents()
{
	return events;
}

void Sequence::startNote(int position, SeqEvent s)
{
	TRACE("setup", "starting note");
	TRACE("setup", "at position:");
	TRACE("setup", position);
	// position means the beat or whatever.
	// 1 e + a 2 e + a 3 e  +  a  4  e  +  a  tradition count
	// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 position
	// we want to insert this SeqEvent at position
	// extending the vector if necessary
	if (position >= events.size()) {
		TRACE("setup", "resizing");
		// +1 because 0 * 2 is still 0
		events.resize(events.size() * 2 + 1);
	}
	//events[position] is a vector of SeqEvents
	TRACE("setup", "pushing");
	TRACE("setup", "events.size()");
	TRACE("setup", events.size());
	events[position].push_back(s);
	TRACE("setup", "done pushing");
}

void Sequence::extendPreviousNote(int position, int notenum, string instrument)
{
	int expected_duration = 1;
	for (int i = position - 1 ; i > 0 ; i++) {
		vector<SeqEvent>::iterator it;
		for (it = events[i].begin() ; it != events[i].end() ; it++) {
			if (it->getNote() == notenum) {
				// found it, check the duration
				if (it->getDuration() != expected_duration) {
					// error
					exit(1);
				}
				it->setDuration(it->getDuration() + 1);
			}
		}
		expected_duration++;
	}
	//error, got to beginning w/o finding it
	exit(1);
}

int Sequence::getTicksPerSecond()
{
	return 0;
}

int Sequence::getLength()
{
	return events.size();
}

void Sequence::removeAllButOneInstrument(string iname)
{
	for (int i = 0 ; i < events.size(); i++) {
		vector<SeqEvent>::iterator it;
		vector<SeqEvent> newvect;
		for (it = events[i].begin() ; it != events[i].end() ; it++) {
			if (it->getInstrument() == iname) {
				newvect.push_back(*it);
			}
		}
		events[i] = newvect;
	}
}
