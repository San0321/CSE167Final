#pragma once
#define SOUND_H

#include <GL/glew.h>

#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <al.h>
#include <alc.h>
#include <cstring>
#include <string.h>

class Sound
{
public:
	Sound();
	void createSource(const char* filename);
	void playSource();
	bool isBigEndian();
	int convertToInt(char * buffer, int len);
	char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);
	~Sound();

	unsigned int bufferid, format;
	unsigned int sourceid;

	ALCdevice* device;
	ALCcontext* context;

	char* data;

};