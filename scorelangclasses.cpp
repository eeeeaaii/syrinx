#include "scorelangclasses.h"


scdirectiveline::scdirectiveline()
{
	linetype = "scdirectiveline";
}

string scdirectiveline::dump()
{
	return string("scdirectiveline\n");
}

/*************************************************/

scmusicline::scmusicline(char* ln)
{
	TRACE("setup", "creating new scmusicline");
	line = string(ln);
	linetype = "scmusicline";
}


string scmusicline::getLine()
{
	return line;
}

string scmusicline::dump()
{
	string r = string("--scmusicline: ");
	r += line;
	//	don't need the following because line has a newline at the end.
//	r += string("\n");
	return r;
}

/*************************************************/

scinsertionline::scinsertionline(char* x)
{
	TRACE("setup", "creating scinsertionline(1)");
	type = 'S';
	left = x;
	linetype = "scinsertionline";
}

scinsertionline::scinsertionline(char* a, char* b, char* c)
{
	TRACE("setup", "creating scinsertionline(2)");
	type = 'O';
	left = a;
	op = b;
	right = c;
	linetype = "scinsertionline";
}

string scinsertionline::dump()
{
	string r = string("--scinsertionline:");
	r = r + "left:" + left + " ";
	r = r + "op:" + op + " ";
	r = r + "right:" + right + " ";
	r += "--\n";
	return r;
}

/*************************************************/

void scchunk::addLine(scline* a)
{
	TRACE("setup", "adding line to scchunk");
	lines.push_back(a);
}

void scchunk::setName(char* n)
{
	TRACE("setup", "setting name of scchunk");
	name = string(n);
}

string scchunk::getName()
{
	TRACE("setup", "getting name of scchunk");
	return name;
}

void scchunk::setHiLevList(schilevlist* _hll)
{
	hll = _hll;
}

string scchunk::getInstrumentName(bracetype c)
{
	// go through hi level list looking for set directives
	// for each set directive, look for "c"
	return hll->getInstrumentName(c);


}

string scchunk::dump()
{
	string r;
	r += "scchunk [";
	r += getName();
	r += "] {\n";
	vector<scline*>::iterator i;
	for (i = lines.begin() ; i != lines.end() ; i++) {
		r += (*i)->dump();
	}
	r += "}\n";
	return r;
}

Sequence scchunk::getSequence(scchunklist* clist)
{
	TRACE("setup", "getting sequence");
	vector<scline*>::iterator i;
	Sequence seq;
	int pos = 0;
	for (i = lines.begin() ; i != lines.end() ; i++, pos++) {
		TRACE("setup", "getting sequence line");
		// for each line, go through it and look for stuff.
		// make a new seqevent for each one.
		if ((*i)->linetype == "scdirectiveline") {
			TRACE("setup", "scdirectiveline");
		} else if ((*i)->linetype == "scinsertionline") {
			TRACE("setup", "scinsertionline");
			// get sequence for the thing
			// and insert after this seq
		} else if ((*i)->linetype == "scmusicline") {
			TRACE("setup", "scmusicline");
			scmusicline *m = (scmusicline*)*i;
			string ln = m->getLine();
			TRACE("setup", ln);
			string::iterator j;
			int note = 0;
			for (j = ln.begin(); j != ln.end() ; j++, note++) {
				//TRACE("setup", "line character");
				char c = *j;
				bracetype b;
				int vel = 1;
				int dur = 1;
				bool start;
				switch(c) {
				case '[':
					b = BRACETYPE_SQUARE;
					start = true;
					break;
				case ']':
					b = BRACETYPE_SQUARE;
					start = false;
					break;
				case '(':
					b = BRACETYPE_PAREN;
					start = true;
					break;
				case ')':
					b = BRACETYPE_PAREN;
					start = false;
					break;
				case '{':
					b = BRACETYPE_CURLY;
					start = true;
					break;
				case '}':
					b = BRACETYPE_CURLY;
					start = false;
					break;
				case '<':
					b = BRACETYPE_ANGLE;
					start = true;
					break;
				case '>':
					b = BRACETYPE_ANGLE;
					start = false;
					break;
				default:
					continue;
				}
				if (start) {
					TRACE("setup", "creating SeqEvent");
					SeqEvent se(note, vel, dur, getInstrumentName(b));
					TRACE("setup", "start note");
					seq.startNote(pos, se);
				} else {
					TRACE("setup", "continued note");
					seq.extendPreviousNote(pos, note, getInstrumentName(b));
				}
			}
		} else {
			TRACE("setup", "unknown line type");
		}
		//string::iterator ch;
		//for (ch = (*i).begin() ; ch != (*i).end ; ch++) {
		//	TRACE("setup", *ch);
		//}
	}
	return seq;
}

/*************************************************/

void scchunklist::addChunk(scchunk* c)
{
	TRACE("setup", "adding chunk to chunklist");
	chunks.push_back(c);
}

scchunk* scchunklist::getChunkByName(string chunkname)
{
	TRACE("setup", "getting chunk by name");
	//vector<scchunk*> chunks;
	vector<scchunk*>::iterator i;
	for (i = chunks.begin() ; i != chunks.end() ; i++) {
		if ((*i)->getName() == chunkname) {
			return (*i);
		}
	}
}

string scchunklist::dump()
{
	string s = string("scchunklist: {\n");
	vector<scchunk*>::iterator i;
	for (i = chunks.begin() ; i != chunks.end() ; i++) {
		//cout << "IT IS\n";
		//cout << (*i)->dump();
		s += (*i)->dump();
	}
	s += "}\n";
	return s;
}

/*************************************************/

void schilevlist::addDir(schilevdirective* d)
{
	TRACE("setup", "adding directive to hilevlist");
	dirs.push_back(d);
}

string schilevlist::getInstrumentName(bracetype b)
{
	TRACE("scorelangclasses", "getting instrument name from hll");
	vector<schilevdirective*>::iterator i;
	for (i = dirs.begin() ; i != dirs.end() ; i++) {
		if ((*i)->dirtype == "scsetdirective") {
			scsetdirective *s;
			s = (scsetdirective*)*i;
			if (s->getType() == b) {
				TRACE("scorelangclasses", string("returning: ") + s->getName());
				return s->getName();
			}
		}
	}
	TRACE("scorelangclasses", "returning nothing!");
	return string("");
}

string schilevlist::dump()
{
	return "schilevlist\n";
}

/*************************************************/

string schilevdirective::dump()
{
	return "schilevdirective\n";
}

/*************************************************/

void scsetdirective::setType(bracetype t)
{
	TRACE("setup", "setting type of set directive");
	type = t;
	dirtype = "scsetdirective";
}

void scsetdirective::setName(char* n)
{
	TRACE("setup", "setting name of set directive");
	name = string(n);
}

bracetype scsetdirective::getType()
{
	return type;
}

string scsetdirective::getName()
{
	return name;
}

string scsetdirective::dump()
{
	return "scsetdirective\n";
}



/*
void scchunk::resolveReferences(scchunklist*)
{
	TRACE("setup", "resolving references for scchunk");
	vector<scline*>::iterator i;
	for (i = lines.begin() ; i != lines.end() ; i++) {
		TRACE("setup", "resolving references for line");
	}
}
*/
