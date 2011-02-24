#include "Output.h"
#include "Link.h"
#include "Logger.h"

#ifdef LINUX

Output::Output()
{
	cout << "constructing output";
	opendevice(&mydev);
	// 2 channels
	channels = 2;
	bufcount = mydev.frames * channels;
	bufsize = bufcount * sizeof(buftype);
	buf = (buftype*)malloc(bufsize);
	current = 0;
	underruns = 0;
}

Output::~Output()
{
	closedevice(&mydev);
	free(buf);
}

void Output::processOneSample()
{
	double ceiling = 30000;
	double sample = inputs.at(0)->getData();
	int rc;
	buftype val = (buftype)(sample * ceiling);
	buf[current] = val;
	buf[current + 1] = val;
	//Logger::log("writing sample");
#ifdef SOUND_DEBUG
	fprintf(stderr, "%d\n", val);
#endif
	if (current == bufcount - 2) {
#ifndef SOUND_DEBUG
		//Logger::log("sending to api");
		rc = snd_pcm_writei(mydev.handle, buf, mydev.frames);
		if (rc == -EPIPE) {
		//	Logger::log("underrun");
			fprintf(stderr, "underrun %d\n", underruns++);
			snd_pcm_prepare(mydev.handle);
		} else if (rc < 0) {
			ALSADIE(rc);
		} else if (rc != (int)mydev.frames) {
			fprintf(stderr, "short write!\n");
		}
#endif
		current = 0;
	} else {
		current += 2;
	}
}

void Output::opendevice(Device *dev)
{
	int rc;
	int i;
	unsigned int val, val2;
	int dir;
	int size;
	int f;
	char *buffer;

	// open
	rc = snd_pcm_open(&(dev->handle), "default", SND_PCM_STREAM_PLAYBACK, 0);
	if (rc < 0) ALSADIE(rc);
	// allocate mem for params struct
	snd_pcm_hw_params_alloca(&(dev->params));
	// fill w/ default values
	snd_pcm_hw_params_any(dev->handle, dev->params);
	// set desired attributes in params struct
	snd_pcm_hw_params_set_access(dev->handle, dev->params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(dev->handle, dev->params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_channels(dev->handle, dev->params, 2);
	val = SRATE;
	snd_pcm_hw_params_set_rate_near(dev->handle, dev->params, &val, &dir);
	dev->frames = 32;
	snd_pcm_hw_params_set_period_size_near(dev->handle, dev->params, &(dev->frames), &dir);
	// put params into driver
	rc = snd_pcm_hw_params(dev->handle, dev->params);
	if (rc < 0) ALSADIE(rc);
	// initialize "frames" member of device
	snd_pcm_hw_params_get_period_size(dev->params, &(dev->frames), &dir);
}

void Output::closedevice(Device *dev)
{
	snd_pcm_drain(dev->handle);
	snd_pcm_close(dev->handle);
}

string Output::getInfo()
{
	return string("Output module.");
}

#endif
