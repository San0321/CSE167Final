#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

const int NUMBER_OF_CURVE = 8;
const int LINE_SEGMENT = 150;

class Curve
{
public:
	Curve();
	~Curve();

	void Update();
	void Draw(GLuint shaderProgram);

	glm::vec3 GetPoint(int curveID, float t);

	void EnableC1Continuity();

	void NextControlPoint();
	void PrevControlPoint();
	
	int GetControlPointID() const;
	void MovePoint(glm::vec3& v);

	//void MoveX();
	//void MoveY();
	//void MoveZ();

private:
	void createCurve();

	int controlPoint;
	int currentCurve;

	GLuint curveVAO, curveVBO;
	unsigned int mVertexCount;

	std::vector<glm::vec3> points;
	GLuint pointVAO, pointVBO;
};