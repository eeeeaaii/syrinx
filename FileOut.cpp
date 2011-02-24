#include "FileOut.h"
#include "Link.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/uio.h"

FileOut::FileOut()
{
	fp = 0;
	filename = string("out.raw");
}

FileOut::~FileOut()
{
	if (fp) {
		fclose(fp);
	}
}

void FileOut::processOneSample()
{
	if (!fp) {
		fp = fopen(filename.c_str(), "w");
	}
	double output = 0;
	double data;
	int sz;
	sz = inputs.size();
	for (int i = 0 ; i < sz ; i++) {
		output += inputs.at(i)->getData() / sz;
	}
	//static int COUNTER = 0;
	//printf("%f\n", output);
	//if (COUNTER == 0) getchar();
	//COUNTER = (COUNTER + 1) % 100;
	double ceiling = 30000.0;
	short val = (short)(output * ceiling);
	//printf("  %d\n", val);
	fputc(val&0x00ff, fp);
	fputc(val>>8, fp);
}

void FileOut::setStringParameter(string pname, string value)
{
	if (pname == string("filename")) {
		filename = value;
	} else {
		cout << "invalid parameter.\n";
		exit(1);
	}
}


string FileOut::getInfo()
{
	return string("FileOut module");
}

void FileOut::finalize()
{
	// get size of out.raw
	int fd;
	char buf[64];
	int bytesread;
	int totalsize = 0;
	fd = open("out.raw", O_RDONLY, 0);
	while((bytesread = read(fd, buf, BUFSIZE)) == BUFSIZE) totalsize += BUFSIZE;
	lseek(fd, 0, SEEK_SET);

	int outfd;
	outfd = open("out.wav", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);

	FormatChunk fchunk;
	DataChunk dchunk = DataChunk(totalsize);
	write(outfd, &fchunk, sizeof(fchunk));
	write(outfd, &dchunk, sizeof(dchunk));
	while((bytesread = read(fd, buf, BUFSIZE)) == BUFSIZE) {
		write(outfd, buf, bytesread);
	}
	close(fd);
	close(outfd);
}

DataChunk::DataChunk(long s)
{
	chunkID[0] = 'd';
	chunkID[1] = 'a';
	chunkID[2] = 't';
	chunkID[3] = 'a';
	size = s;
}

FormatChunk::FormatChunk()
{
	chunkID[0] = 'f';
	chunkID[1] = 'm';
	chunkID[2] = 't';
	chunkID[3] = ' ';
	chunksize = 16;
	formattag = 1;
	channels = 1;
	samplespersec = 44100;
	bitspersample = 16;
	blockalign = channels * (bitspersample / 8);
	avgbytespersec = samplespersec * blockalign;
}
