#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/soundcard.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#define BUFLEN 2048

#define DIE(err) { perror("") ; printf("Exiting with errno of %d at line %d\n", err, __LINE__); exit(1); }

#define PI 3.14159

double absval(double in);

typedef short buftype;

int main(int argc, char **argv)
{
	buftype buf[BUFLEN * sizeof(buftype)];
	int err;
	int myerrno;
	int fdaudio;
	ssize_t n;

	double ceiling = 32768;
	double offset = 32768;
	double samplingrate = 44100;
	double frequency = 880;
	double samplespercycle = samplingrate / frequency;
	double increment = PI / samplespercycle;
	double i;
	int bufi;
	int status;
	int arg;

	// frequency is cycles/sec
	// one cycle is the iterator counting from zero to PI
	// we want to play A440
	// I am assuming that it's a 16 bit card, with
	// 44.1 kHz sampling rate, i.e. 44100 samples/second
	// so it's 44100/440 samples per cycle
	// but we're not done.
	// if we want to go from 0 to PI in N samples,
	// each time we increment by PI/N samples.

	// open device
	fdaudio = open("/dev/snd/pcmC0D0p", O_RDWR, 0);
	if (fdaudio == -1) DIE(errno);

	// set 16 bit samples
	arg = 16;
	status = ioctl(fdaudio, SOUND_PCM_WRITE_BITS, &arg);
	if (status == -1) DIE(errno);

	// set 2 channels
	arg = 2;
	status = ioctl(fdaudio, SOUND_PCM_WRITE_CHANNELS, &arg);
	if (status == -1) DIE(errno);

	// set sample rate
	arg = samplingrate;
	status = ioctl(fdaudio, SOUND_PCM_WRITE_RATE, &arg);
	if (status == -1) DIE(errno);

	for ( i = 0, bufi = 0 ; ; i += increment , bufi = (bufi + 2) % BUFLEN) {
		double val = (buftype)((sin(i) * ceiling) + offset);
		buf[bufi] = val;
		buf[bufi + 1] = val;
		if (bufi == BUFLEN - 2) {
			err = write(fdaudio, buf, (BUFLEN * sizeof(buftype)));
			if (err == -1) DIE(errno);
		}
	}
}

