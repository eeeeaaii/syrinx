#pragma once
#include "standardincludes.h"


class SeqEvent {
private:
	int notenum;
	int velocity;
	int duration;
	string instrument;
public:
	SeqEvent();
	SeqEvent(int, int, int, string);
	SeqEvent(SeqEvent&);
	SeqEvent(const SeqEvent&);
	SeqEvent& operator=(SeqEvent&);
	SeqEvent& operator=(const SeqEvent&);
	void setNote(int);
	void setDuration(int);
	void setVelocity(int);
	void setInstrument(string);
	/*
	int getNote();
	int getDuration();
	int getVelocity();
	string getInstrument();
	*/
	int getNote() const;
	int getDuration() const;
	int getVelocity() const;
	string getInstrument() const;
};



