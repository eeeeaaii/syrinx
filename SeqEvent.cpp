#include "SeqEvent.h"

SeqEvent::SeqEvent()
{
	setNote(0);
	setVelocity(0);
	setDuration(0);
	setInstrument(string(""));
}

SeqEvent::SeqEvent(int note, int vel, int dur, string inst)
{
	setNote(note);
	setVelocity(vel);
	setDuration(dur);
	setInstrument(inst);
}

SeqEvent::SeqEvent(SeqEvent& rhs)
{
	setNote(rhs.getNote());
	setVelocity(rhs.getVelocity());
	setDuration(rhs.getDuration());
	setInstrument(rhs.getInstrument());
}

SeqEvent::SeqEvent(const SeqEvent& rhs)
{
	setNote(rhs.getNote());
	setVelocity(rhs.getVelocity());
	setDuration(rhs.getDuration());
	setInstrument(rhs.getInstrument());
}

SeqEvent& SeqEvent::operator=(SeqEvent& rhs)
{
	if (&rhs != this) {
		setNote(rhs.getNote());
		setVelocity(rhs.getVelocity());
		setDuration(rhs.getDuration());
		setInstrument(rhs.getInstrument());
	}
	return *this;
}

SeqEvent& SeqEvent::operator=(const SeqEvent& rhs)
{
	if (&rhs != this) {
		setNote(rhs.getNote());
		setVelocity(rhs.getVelocity());
		setDuration(rhs.getDuration());
		setInstrument(rhs.getInstrument());
	}
	return *this;
}

void SeqEvent::setNote(int note)
{
	notenum = note;
}

void SeqEvent::setVelocity(int vel)
{
	velocity = vel;
}

void SeqEvent::setDuration(int dur)
{
	duration = dur;
}

void SeqEvent::setInstrument(string inst)
{
	instrument = inst;
}

/*
int SeqEvent::getNote()
{
	return notenum;
}

int SeqEvent::getVelocity()
{
	return velocity;
}

int SeqEvent::getDuration()
{
	return duration;
}

string SeqEvent::getInstrument()
{
	return instrument;
}

*/

int SeqEvent::getNote() const
{
	return notenum;
}

int SeqEvent::getVelocity() const
{
	return velocity;
}

int SeqEvent::getDuration() const
{
	return duration;
}

string SeqEvent::getInstrument() const
{
	return instrument;
}
