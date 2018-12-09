#include "BezierCurve.h"

#include <glm/glm.hpp>

#include "Window.h"

Bezier::Bezier()
{
	points = {
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

	//EnableC1Continuity();

	for (int i = 0; i < NUMBER_OF_CURVE; i++)
	{
		int id0 = (i * 3) + 1;
		int id1 = id0 + 1;
		int id2 = (id1 + 1) % 24;
		int id3 = id2 + 1;

		for (int j = 0; j < LINE_SEGMENT; j++)
		{
			float t = (float)j / (float)LINE_SEGMENT;
			float t2 = t * t;
			float t3 = t * t * t;

			float B0 = -t3 + (3.0f * t2) - (3 * t) + 1;
			float B1 = (3.0f * t3) - (6.0f * t2) + (3.0f * t);
			float B2 = -(3.0f * t3) + (3.0f * t2);
			float B3 = t3;

			vertices.push_back(points[id0] * B0 + points[id1] * B1 + points[id2] * B2 + points[id3] * B3);
		}
	}
	mVertexCount = (unsigned int)vertices.size();

	// Curve
	glGenVertexArrays(1, &mCurveVao);
	glGenBuffers(1, &mCurveVbo);

	glBindVertexArray(mCurveVao);

	glBindBuffer(GL_ARRAY_BUFFER, mCurveVbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Point
	glGenVertexArrays(1, &mPointVao);
	glGenBuffers(1, &mPointVbo);

	glBindVertexArray(mPointVao);

	glBindBuffer(GL_ARRAY_BUFFER, mPointVbo);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Bezier::~Bezier()
{
	glDeleteVertexArrays(1, &mCurveVao);
	glDeleteBuffers(1, &mCurveVbo);

	glDeleteVertexArrays(1, &mPointVao);
	glDeleteBuffers(1, &mPointVbo);
}

void Bezier::Draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);

	glm::mat4 wvp = Window::P * Window::V * glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "WorldViewProjection"), 1, GL_FALSE, &wvp[0][0]);

	GLuint rgb = glGetUniformLocation(shaderProgram, "RGB");

	// Draw Lines
	glBindVertexArray(mCurveVao);
	glUniform3f(rgb, 0.0f, 0.0f, 0.0f);
	glDrawArrays(GL_LINE_STRIP, 0, mVertexCount);
	glBindVertexArray(0);

	// Draw Points
	glBindVertexArray(mPointVao);

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

	glBindVertexArray(0);
}

void Bezier::EnableC1Continuity()
{
	for (int i = 0; i < NUMBER_OF_CURVE; ++i)
	{
		int c0 = (i * 3);
		int c1 = (c0 + 1) % 24;

		points[c1 + 1] = points[c1] + (points[c1] - points[c0]);
	}
}