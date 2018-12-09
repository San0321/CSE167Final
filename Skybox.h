#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Skybox
{
public:
	Skybox();
	~Skybox();

	virtual void Update();
	virtual void Draw(GLuint shaderProgram);

	unsigned int texture;
	GLuint skyboxVAO, skyboxVBO;

	GLuint uWorldViewProjection;
private:
	unsigned char * skyBoxLoadPPM(const char* filename, int& width, int& height);
	unsigned int Skybox::loadCubemap(std::vector<std::string> faces, int twidth, int theight);
	unsigned int cubemapTexture;
	//std::vector<glm::vec3> vertices;
};

#endif