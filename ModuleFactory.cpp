#include "standardincludes.h"
#include "ModuleFactory.h"
#include "Module.h"
#include "Oscillator.h"
#include "Output.h"
#include "FileOut.h"
#include "FunctionModule.h"
#include "Link.h"
#include "Sequencer.h"
#include "Delay.h"
#include "Filter.h"
#include "Mixer.h"
#include "scorelangclasses.h"
#include "OrchTranslator.h"
#include "Configuration.h"

extern FILE* yyin;
extern FILE* scyyin;
extern OrchTranslator* gl_orchTranslator;
extern scchunklist *scresult;

void ModuleFactory::setupModules(modGroup& mods, linkGroup& links)
{
	TRACE("setup", "parsing score file");
	// parse scorefile
	string scorefile = Configuration::getInstance()->scoreFileName;
	scyyin = fopen(scorefile.c_str(), "r");
	do {
		scyyparse();
	} while(!feof(scyyin));
	if (!scresult) exit(0); // nothing to do!

	TRACE("setup", "parsing orch file");
	// parse orchfile
	string orchfile = Configuration::getInstance()->orchFileName;
	yyin = fopen(orchfile.c_str(), "r");
	do {
		yyparse();
	} while(!feof(yyin));
	if (!gl_orchTranslator) exit(0); // nothing to do!
	//cerr << yresultf->toString();

	if (Configuration::getInstance()->printScoreAST) {
		cout << scresult->dump();
	}
	if (Configuration::getInstance()->printOrchAST) {
		cout << gl_orchTranslator->dump();
	}

	// now go into the score and get sequence
	TRACE("setup", "getting \"main\" chunk from score file");
	scchunk* mainchunk = scresult->getChunkByName("main");
	TRACE("setup", "resolving references for main chunk");
	Sequence masterseq = mainchunk->getSequence(scresult);

	TRACE("setup", "sequence length:");
	TRACE("setup", masterseq.getLength());

	TRACE("setup", "getting instruments used");
	set<string> instr = masterseq.getInstrumentsUsed();

	set<string>::iterator vsi;
	for (vsi = instr.begin() ; vsi != instr.end() ; vsi++) {

		TRACE("ModuleFactory", string("doing instrument " + *vsi));
		Sequencer *sequ = new Sequencer();
		Sequence seq = masterseq;
		/*
		 this appears to be a bug
		 if there are three instruments, foo, bar and baz,
		 and the first time I remove all but foo,
		 then the only thing left is foo.
		 the second time, when I remove all but bar,
		 there will be nothing left.
		 I think what I meant to do here is copy it.
		 *
		 */
		seq.removeAllButOneInstrument(*vsi);
		sequ->setSequence(seq);
		mods.push_back(sequ);

		Link *note = new Link();
		Link *trig = new Link();
		Link *velo = new Link();

		FunctionModule *fm = gl_orchTranslator->translate(*vsi, note, trig, velo);
		TRACE("ModuleFactory", "translation complete");
		if (note->hasOutput()) {
			TRACE("ModuleFactory", string("a"));
			sequ->addOutput(note);
			links.push_back(note);
		} else {
			TRACE("ModuleFactory", string("a2"));
			delete note;
		}
		if (trig->hasOutput()) {
			TRACE("ModuleFactory", string("b"));
			sequ->addOutput(trig);
			links.push_back(trig);
		} else {
			TRACE("ModuleFactory", string("b2"));
			delete trig;
		}
		if (velo->hasOutput()) {
			TRACE("ModuleFactory", string("c"));
			sequ->addOutput(velo);
			links.push_back(velo);
		} else {
			TRACE("ModuleFactory", string("c2"));
			delete velo;
		}

		TRACE("ModuleFactory", string("finish " + *vsi));
		mods.push_back(fm);


		/*
		// wire up -- sorry, no error checking yet.
		Link *note = new Link();
		Link *trig = new Link();
		Link *velo = new Link();
		sequ->addOutput(note);
		sequ->addOutput(trig);
		sequ->addOutput(velo);
		Module* notemod = fm->getNextInnerInput();
		Module* trigmod = fm->getNextInnerInput();
		Module* velomod = fm->getNextInnerInput();
		notemod->addInput(note);
		trigmod->addInput(trig);
		velomod->addInput(velo);
	*/
	}
	TRACE("setup", string("done"));
}
