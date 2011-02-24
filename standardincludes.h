#pragma once

//#define LINUX
#define WIN32_CYGWIN

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#ifdef LINUX
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <linux/soundcard.h>
#endif
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

//#define SOUND_DEBUG
//#define TRACE(x)
#define TRACE(x, y) debug((x), (y));


using namespace std;

class Module;
class Link;

typedef vector<Module*> modGroup;
typedef vector<Module*>::iterator modGroupIter;

typedef vector<Link*> linkGroup;
typedef vector<Link*>::iterator linkGroupIter;
typedef map<string, Link*> linkMap;
typedef map<string, Link*>::iterator linkMapIter;

typedef vector<double> vdouble;
typedef vector<double>::iterator vdoublei;
typedef vector<int> vint;
typedef vector<int>::iterator vinti;

#define SRATE 44100
#define CRATE 441

typedef enum {
	BRACETYPE_PAREN = 1,
	BRACETYPE_CURLY,
	BRACETYPE_SQUARE,
	BRACETYPE_ANGLE
} bracetype;

enum IdentifierType {
	LINK,  // @whatever
	STRING, // "whatever"
	METALINK,  // @@whatever
	METASTRING,  // #whatever
	ANONLINK  // %%whatever
}; 

typedef enum {
	WAVE_SINE = 0,
	WAVE_SQUARE,
	WAVE_SAW
} wavetype;

typedef enum {
	PTYPE_UNKNOWN = 0,
	PTYPE_LINK,
	PTYPE_ENUMERATION,
	PTYPE_STRING
} parameterType;

double hzToSignal(double f);
double signalToHz(double);
double toDouble(string s);

void debug(string, string);
void debug(string, int);
