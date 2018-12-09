#include "Sound.h"

#include <iostream>
#include <fstream>
#include <stdio.h>


bool Sound::isBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

int Sound::convertToInt(char* buffer, int len)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i < len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

char* Sound::loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "this is not a valid WAVE file" << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);      //WAVE
	in.read(buffer, 4);      //fmt
	in.read(buffer, 4);      //16
	in.read(buffer, 2);      //1
	in.read(buffer, 2);
	chan = convertToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4);      //data
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);
	return data;
}

Sound::Sound()
{
	device = alcOpenDevice(NULL);
	if (device == NULL)
	{
		std::cout << "cannot open sound card" << std::endl;
	}
	context = alcCreateContext(device, NULL);
	if (context == NULL)
	{
		std::cout << "cannot open context" << std::endl;
	}
	alcMakeContextCurrent(context);
}

void Sound::createSource(const char* filename) {

	int channel, sampleRate, bps, size;

	char* data = loadWAV("puzzle.wav", channel, sampleRate, bps, size);

	std::cout << "Successfully loaded WAV file" << std::endl;


	alGenSources(1, &sourceid);
	alGenBuffers(1, &bufferid);
	

	if (channel == 1)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else {
			format = AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else {
			format = AL_FORMAT_STEREO16;
		}
	}

	alBufferData(bufferid, format, data, size, sampleRate);
	
	alSourcei(sourceid, AL_BUFFER, bufferid);
	alSourcei(sourceid, AL_LOOPING, AL_TRUE);
}


void Sound::playSource() {
	alSourcePlay(this->sourceid);
}


Sound::~Sound() {
	alDeleteSources(1, &sourceid);
	alDeleteBuffers(1, &bufferid);

	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
	delete[] this->data;
}