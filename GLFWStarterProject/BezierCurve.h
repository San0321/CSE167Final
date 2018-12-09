#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

const int NUMBER_OF_CURVE = 8;
const int LINE_SEGMENT = 150;

class Bezier
{
public:
	Bezier();
	~Bezier();

	void Draw(GLuint shaderProgram);

	void EnableC1Continuity();

private:

	GLuint mCurveVao, mCurveVbo;
	unsigned int mVertexCount;

	std::vector<glm::vec3> points;
	GLuint mPointVao, mPointVbo;
};