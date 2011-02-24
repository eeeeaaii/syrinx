#include "Sequencer.h"
#include "Link.h"

Sequencer::Sequencer()
{
	tickcounter = 0;
	current = 0;
	ticksperchange = 0;
}

void Sequencer::processOneControlEvent()
{
	if (ticksperchange == 0) {
		//cerr << "Error: can't play without a sequence.\n";
		exit(1);
	}
	if (current > seq.getLength()) return;
	if (tickcounter == 0) {
		/*
		if (events.at(current)) {
			// this assumes we are sending the frequency over, not the note number.
			double f = events.at(current)->getFrequency();
			// first map 0hz to 25000hz -> 0 to 1
			f /= 25000.0;
			// then map 0 to 1 -> 0 to 2
			f *= 2.0;
			// finally map 0 to 2 -> -1 to 1
			f -= 1.0;
			outputs.at(0)->putData(f);
		}
		*/
		current++;
	}
	tickcounter = (tickcounter + 1) % ticksperchange;
}

void Sequencer::setSequence(Sequence s)
{
	int t = s.getTicksPerSecond();
	ticksperchange = (int)((double)CRATE / (double)t);
}


string Sequencer::getInfo()
{
	return string("Sequencer module.");
}

/*
 * we do this in 16th notes
 * MIDI note 0 is 8.175 Hz
 * to get from C to C#, multiply by pow(2, (1 / 12))
 * to get from C to D, multiply by pow(pow(2, 2), (1/12)) or pow(2, 6)
 * to get from C to D#, multiply by pow(pow(2, 3), (1/12))
 * 
 */

NoteMapper::NoteMapper()
{
	double currentval = 8.175;
	double ratio = pow(2.0, (1.0 / 12.0));
	notefrequencies.push_back(currentval);
	for (int i = 1 ; i < 128 ; i++) {
		notefrequencies.push_back(currentval);
		currentval *= ratio;
	}
}

double NoteMapper::mapFrequency(int note)
{
	return notefrequencies[note];
}

