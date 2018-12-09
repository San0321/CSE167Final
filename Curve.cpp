#include "Curve.h"
#include "Window.h"

Curve::Curve() : controlPoint(1), currentCurve(0)
{
	fprintf(stderr, "Curve Constructor\n");

	points = {
		// positions      
		glm::vec3(-3.0f, 1.0f, 0.0f),
		glm::vec3(-6.0f, 3.0f, 0.0f),
		glm::vec3(-9.0f, 8.0f, 0.0f),
		glm::vec3(-12.0f, 14.0f, 0.0f),
		glm::vec3(-15.0f, 20.0f, 0.0f),
		glm::vec3(-18.0f, 26.0f, 0.0f),
		glm::vec3(-18.0f, 29.0f, 0.0f),
		glm::vec3(-15.0f, 31.0f, 0.0f),
		glm::vec3(-12.0f, 35.0f, 0.0f),
		glm::vec3(-9.0f, 38.0f, 0.0f),
		glm::vec3(-6.0f, 44.0f, 0.0f),
		glm::vec3(-3.0f, 50.0f, 0.0f),

		glm::vec3(3.0f, 50.0f, 0.0f),
		glm::vec3(6.0f, 44.0f, 0.0f),
		glm::vec3(9.0f, 38.0f, 0.0f),
		glm::vec3(12.0f, 35.0f, 0.0f),
		glm::vec3(15.0f, 31.0f, 0.0f),
		glm::vec3(18.0f, 29.0f, 0.0f),
		glm::vec3(18.0f, 26.0f, 0.0f),
		glm::vec3(15.0f, 20.0f, 0.0f),
		glm::vec3(12.0f, 14.0f, 0.0f),
		glm::vec3(9.0f, 8.0f, 0.0f),
		glm::vec3(6.0f, 3.0f, 0.0f),
		glm::vec3(3.0f, 1.0f, 0.0f)
	};

	EnableC1Continuity();

	// Curve
	glGenVertexArrays(1, &curveVAO);
	glGenBuffers(1, &curveVBO);

	// Point
	glGenVertexArrays(1, &pointVAO);
	glGenBuffers(1, &pointVBO);

	createCurve();
}

Curve::~Curve()
{
	glDeleteVertexArrays(1, &curveVAO);
	glDeleteBuffers(1, &curveVBO);

	glDeleteVertexArrays(1, &pointVAO);
	glDeleteBuffers(1, &pointVBO);
}


void Curve::Update()
{
}

void Curve::Draw(GLuint curveShaderProgram)
{
	glUseProgram(curveShaderProgram);

	glm::mat4 wvp = Window::P * Window::V * glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(curveShaderProgram, "WorldViewProjection"), 1, GL_FALSE, &wvp[0][0]);

	GLuint rgb = glGetUniformLocation(curveShaderProgram, "RGB");

	// Draw Lines
	glBindVertexArray(curveVAO);
	glUniform3f(rgb, 0.0f, 0.0f, 0.0f);
	glDrawArrays(GL_LINE_STRIP, 0, mVertexCount);
	glBindVertexArray(0);

	// Draw Points
	glBindVertexArray(pointVAO);

	for (int i = 0; i < NUMBER_OF_CURVE; ++i)
	{
		glPointSize(12.0f);
		glUniform3f(rgb, 1.0f, 0.0f, 0.0f);
		glDrawArrays(GL_POINTS, (i * 3) + 1, 1);

		glPointSize(8.0f);
		glUniform3f(rgb, 0.0f, 1.0f, 0.0f);
		glDrawArrays(GL_POINTS, (i * 3), 1);
		glDrawArrays(GL_POINTS, (i * 3) + 2, 1);

		glUniform3f(rgb, 1.0f, 1.0f, 0.0f);
		glDrawArrays(GL_LINE_STRIP, (i * 3), 3);
	}

	glPointSize(15.0f);
	glUniform3f(rgb, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_POINTS, controlPoint, 1);

	glBindVertexArray(0);
}

void Curve::EnableC1Continuity()
{
	for (int i = 0; i < NUMBER_OF_CURVE; ++i)
	{
		int c0 = (i * 3);
		int c1 = (c0 + 1) % 24;

		points[c1 + 1] = points[c1] + (points[c1] - points[c0]);
	}
}

void Curve::NextControlPoint()
{
	controlPoint += 1;
	if (controlPoint > 23)
	{
		controlPoint = 0;
	}

	currentCurve = controlPoint / 3;
}

void Curve::PrevControlPoint()
{
	controlPoint -= 1;
	if (controlPoint < 0)
	{
		controlPoint = 23;
	}

	currentCurve = controlPoint / 3;
}

int Curve::GetControlPointID() const
{
	return controlPoint;
}

void Curve::MovePoint(glm::vec3& v)
{

	// Anchor Point should move all three points
	if (controlPoint == (currentCurve * 3) + 1)
	{
		points[controlPoint]     += v;
		points[controlPoint - 1] += v;
		points[controlPoint + 1] += v;
	}
	// First Pull Point should move first and the second Pull point // c1 continuious
	else if (controlPoint == (currentCurve * 3))
	{
		points[controlPoint] += v;
		points[controlPoint + 2] = points[controlPoint + 1] + (points[controlPoint + 1] - points[controlPoint]);
	}
	// Second Pull Point Should Move Second and the First Pull Point // c1 continuious 
	else if (controlPoint == (currentCurve * 3) + 2)
	{
		points[controlPoint] += v;
		points[controlPoint - 2] = points[controlPoint - 1] + (points[controlPoint - 1] - points[controlPoint]);
	}

	createCurve();
}

glm::vec3 Curve::GetPoint(int curveID, float t)
{
	int id0 = (curveID * 3) + 1;
	int id1 = id0 + 1;
	int id2 = (id1 + 1) % 24;
	int id3 = id2 + 1;

	float t2 = t * t;
	float t3 = t * t * t;

	float B0 = -t3 + (3.0f * t2) - (3 * t) + 1;
	float B1 = (3.0f * t3) - (6.0f * t2) + (3.0f * t);
	float B2 = -(3.0f * t3) + (3.0f * t2);
	float B3 = t3;

	return points[id0] * B0 + points[id1] * B1 + points[id2] * B2 + points[id3] * B3;
}

void Curve::createCurve()
{
	std::vector<glm::vec3> vertices;
	for (int i = 0; i < NUMBER_OF_CURVE; ++i)
	{
		for (int j = 0; j < LINE_SEGMENT; ++j)
		{
			vertices.push_back(GetPoint(i, (float)j / ((float)LINE_SEGMENT - 1.0f)));
		}
	}

	mVertexCount = (unsigned int)vertices.size();

	// Curve
	glBindVertexArray(curveVAO);

	glBindBuffer(GL_ARRAY_BUFFER, curveVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Points
	glBindVertexArray(pointVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}