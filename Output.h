#pragma once
#include "standardincludes.h"
#include "Module.h"

#ifdef LINUX
#define SYSDIE(err) { perror("") ; fprintf(stderr, "Exiting with errno of %d at line %d\n", err, __LINE__); exit(1); }
#define ALSADIE(err) { fprintf(stderr, "ALSA error: %s at line %d\n", snd_strerror(err), __LINE__); exit(1); }

typedef short buftype;

class Output : public Module
{
private:
	buftype *buf;
	class Device {
	public:
		snd_pcm_t *handle;
		snd_pcm_hw_params_t *params;
		snd_pcm_uframes_t frames;
	}; 
	Device mydev;
	int current;
	int channels;
	int bufcount;
	int bufsize;
	int underruns;
	void opendevice(Device *dev);
	void closedevice(Device *dev);
protected:
	virtual int getMaxOutputs() { return 0;}
	virtual int getMaxInputs() { return 1; }
public:
	Output();
	virtual ~Output();
	virtual void processOneSample();
	virtual string getInfo();
	virtual string nameDebug() { return string("output"); };
};

#endif

#ifdef WIN32_CYGWIN
class Output : public Module
{
	public:
	virtual int getMaxOutputs() { return 0;}
	virtual int getMaxInputs() { return 1; }
	virtual string getInfo() { return string(""); }
	virtual string nameDebug() { return string("output"); };
};
#endif
