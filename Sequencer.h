#pragma once
#include "standardincludes.h"
#include "Module.h"
#include "Sequence.h"


/*
a sequence is basically a vector of vector of events,
where an event is:
1. a note
2. a velocity
3. an instrument

You can't have infinite polyphony.  To have reasonable polyphony,
the engine has to duplicate the graph for each voice.  Or,
it doesn't, and the user assigns each voice to an input
somewhere on the graph.

The sequencer has to be a module because it needs to output
two things: triggers for the envelopes, and pitches for the
oscillators.  Velocity values also go to the envelopes.  Anyway,
so you want to be able to specify an instrument in the sequence.

I guess what you can do is define a top-level function module
to be one that has three inputs: velocity, pitch, and trigger.
So a top-level function module is essentially an instrument.
We create an instance of this function for each voice
that is assigned to that instrument name.

*/

class NoteMapper
{
private:
	vdouble notefrequencies;
public:
	NoteMapper();
	double mapFrequency(int note);
};



class Sequencer : public Module
{
protected:
	virtual int getMaxOutputs() { return 1;}
	virtual int getMaxInputs() { return 0; }
	virtual string nameDebug() { return string("sequencer"); }
	int ticksperchange;
	int tickcounter;
	int current;

	Sequence seq;
	NoteMapper notemapper;

public:
	Sequencer::Sequencer();
	virtual void processOneControlEvent();
	void setSequence(Sequence s);
	virtual string getInfo();
};

