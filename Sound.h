#pragma once
#ifndef SOUND_H
#define SOUND_H

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <al.h>
#include <alc.h>
#include <cstring>
#include <string.h>
#include <map>


class Sound
{
public:
	Sound();
	void createSource(char* name, const char* filename, bool isLoopOn);
	void playSource(char* name);
	bool isBigEndian();
	int convertToInt(char * buffer, int len);
	char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);
	~Sound();

	// key: <source, buffer>
	std::map<std::string, std::pair<ALuint, ALuint>> sources_;

	ALCdevice* device;
	ALCcontext* context;
	unsigned int bufferid, sourceid, format;

	int channel, sampleRate, bps, size;
	char* data;

};
#endif