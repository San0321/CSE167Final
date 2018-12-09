#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Sphere
{
public:
	Sphere();
	~Sphere();

	void Draw(GLuint shaderProgram);
	void Translate(glm::vec3 v);

private:
	GLuint mVAO;
	GLuint mVBO;
	GLuint mNBO;
	GLuint mEBO;

	glm::mat4 toWorld;

	unsigned int mIndicesCount;

	void parse(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices);
};

