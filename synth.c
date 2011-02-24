#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#define SYSDIE(err) { perror("") ; fprintf(stderr, "Exiting with errno of %d at line %d\n", err, __LINE__); exit(1); }
#define ALSADIE(err) { fprintf(stderr, "ALSA error: %s at line %d\n", snd_strerror(err), __LINE__); exit(1); }

#define SRATE 44100
#define BITSPS 16
#define CHANNELS 2
#define PI 3.14159

struct device {
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
}; 
typedef struct device Device;

typedef short buftype;

void listInfo(void);
void opendevice(Device *dev);
void closedevice(Device *dev);
void writetodevice(Device *dev);

int main(int argc, char** argv)
{
	Device dev;
	opendevice(&dev);
	writetodevice(&dev);
	closedevice(&dev);
}

void writetodevice(Device *dev)
{
	int bufsize = dev->frames * 4 / sizeof(buftype);
	//int bufsize = dev->frames * 4 / sizeof(buftype) * 65536;
	buftype buf[bufsize];

	double ceiling = 30000;
	double offset = 32768;
	double frequency = 440;
	double samplespercycle;
	double increment;
	double i;
	int bufi;
	int rc;
	int count = 0;
	int max = 300;

	//printf("bufsize: %d\n", bufsize);

	// frequency is cycles/sec
	// one cycle is the iterator counting from zero to PI
	// we want to play A440
	// I am assuming that it's a 16 bit card, with
	// 44.1 kHz sampling rate, i.e. 44100 samples/second
	// so it's 44100/440 samples per cycle
	// but we're not done.
	// if we want to go from 0 to PI in N samples,
	// each time we increment by PI/N samples.

	for (frequency = 20000 ; frequency > 20 ; frequency -= 100 ) {
		samplespercycle = SRATE / frequency;
		increment = PI / samplespercycle;
		count = 0;
		for ( i = 0, bufi = 0 ; count < max ; i += increment , bufi = (bufi + 2) % bufsize) {
			double wave1 = sin(i) * .7 + sin(i*3) * .2 + sin(i*6) * .1 + sin(i*7) * .05 + sin(i*21) * .05;
			double wave2 = sin(i);
			double mult = sin(i/1000);
			double mult2 = sin((i + 500)/1000);
			//the following sounds cool -- why?  Wavewrapping?
			//double wave = wave1 * mult + (wave2 / mult);
			//double wave = wave1 * mult + wave2 * mult2;
			double wave = wave1;
			buftype val = (buftype)(wave * ceiling);
			//buftype val = 0;
			//buftype val = (buftype)((int)(i*100) % 10000);
			//printf("%f %f %f %d %x\n", wave, wave * ceiling, (wave * ceiling) + offset, val, val);
			//val = (val >> 8) | (val << 8);
			buf[bufi] = val;
			buf[bufi + 1] = val;
			//printf("val= %d\n", val);
			if (bufi == bufsize - 2) {
				rc = snd_pcm_writei(dev->handle, buf, dev->frames);
				if (rc == -EPIPE) {
					fprintf(stderr, "underrun on trial %d\n", count);
					snd_pcm_prepare(dev->handle);
				} else if (rc < 0) {
					ALSADIE(rc);
				} else if (rc != (int)dev->frames) {
					fprintf(stderr, "short write!\n");
				}
				count++;
			}
		}
	}
}


void opendevice(Device *dev)
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
	snd_pcm_hw_params_set_channels(dev->handle, dev->params, CHANNELS);
	val = SRATE;
	snd_pcm_hw_params_set_rate_near(dev->handle, dev->params, &val, &dir);
	dev->frames = 32;
	snd_pcm_hw_params_set_period_size_near(dev->handle, dev->params, &(dev->frames), &dir);
	// put params into driver
	rc = snd_pcm_hw_params(dev->handle, dev->params);
	if (rc < 0) ALSADIE(rc);
	// initialize "frames" member of device
	snd_pcm_hw_params_get_period_size(dev->params, &(dev->frames), &dir);
	/*
	size = dev->frames * 4; // with 16bit 2channel we have 4 byte/frame
	buffer = (char*)malloc(size);
	snd_pcm_hw_params_get_period_time(dev->params, &val, &dir);
	for (i = 0 ; i < size ; i++) {
		buffer[i] = (i * 20) % 20000;
	}
	rc = snd_pcm_writei(dev->handle, buffer, dev->frames);
	if (rc == -EPIPE) {
		fprintf(stderr, "underrun!\n");
		snd_pcm_prepare(dev->handle);
	} else if (rc < 0) {
		ALSADIE(rc);
	} else if (rc != (int)dev->frames) {
		fprintf(stderr, "short write!\n");
	}
	snd_pcm_drain(dev->handle);
	snd_pcm_close(dev->handle);
	free(buffer);
	*/
}

void closedevice(Device *dev)
{
	snd_pcm_drain(dev->handle);
	snd_pcm_close(dev->handle);
}


void listInfo() {
	int val;
	printf("alsa lib version: %s\n", SND_LIB_VERSION_STR);
	printf("stream types:\n");
	for (val = 0 ; val < SND_PCM_STREAM_LAST ; val++) {
		printf("  %s\n", snd_pcm_stream_name((snd_pcm_stream_t)val));
	}
	printf("formats:\n");
	for (val = 0 ; val < SND_PCM_FORMAT_LAST ; val++) {
		printf("  %s (%s)\n", snd_pcm_format_name((snd_pcm_format_t)val), snd_pcm_format_description((snd_pcm_format_t)val));
	}
	printf("subformats:\n");
	for (val = 0 ; val < SND_PCM_SUBFORMAT_LAST ; val++) {
		printf("  %s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val), snd_pcm_subformat_description((snd_pcm_subformat_t)val));
	}
	printf("states:\n");
	for (val = 0 ; val < SND_PCM_STATE_LAST ; val++) {
		printf("  %s\n", snd_pcm_state_name((snd_pcm_state_t)val));
	}
}
