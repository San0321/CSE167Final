#include "Sphere.h"

#include <stdio.h>

#include "../Window.h"

#define SPHERE_PATH "./Objects/sphere.obj"

Sphere::Sphere()
{
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, 3.0f, 0.0f));

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	parse(vertices, normals, indices);

	mIndicesCount = indices.size();

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mNBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mNBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mNBO);
	glDeleteBuffers(1, &mEBO);
}

void Sphere::Draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);

	// Shader setup
	glm::mat4 wvp = Window::P * Window::V * toWorld;
	GLuint WorldViewProjection = glGetUniformLocation(shaderProgram, "WorldViewProjection");
	glUniformMatrix4fv(WorldViewProjection, 1, GL_FALSE, &wvp[0][0]);

	glm::vec3 camPos = glm::vec3(Window::V[3][0], Window::V[3][1], Window::V[3][2]);
	GLuint CamPosition = glGetUniformLocation(shaderProgram, "CamPosition");
	glUniform3f(CamPosition, camPos.x, camPos.y, camPos.z);

	// Draw
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::Translate(glm::vec3 v)
{
	toWorld = glm::translate(glm::mat4(1.0f), v);
}

void Sphere::parse(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices)
{
	FILE *fp;
	fp = fopen(SPHERE_PATH, "rb");

	if (fp == nullptr)
	{
		fprintf(stderr, "File Not Found\n");
		exit(-1);
	}

	char buf[64];

	float x, y, z;
	float nx, ny, nz;
	unsigned int v1, v2, v3;

	// Read files
	while (fgets(buf, sizeof(buf), fp))
	{
		if (sscanf(buf, "v %f %f %f", &x, &y, &z))
		{
			// Read vertex
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if (sscanf(buf, "vn %f %f %f", &nx, &ny, &nz))
		{
			// Read vertex normal
			normals.push_back(glm::vec3(nx, ny, nz));
		}
		else if (sscanf(buf, "f %u//%*u %u//%*u %u//%*u", &v1, &v2, &v3))
		{
			// Read indices
			indices.push_back(v2 - 1);
			indices.push_back(v3 - 1);
			indices.push_back(v1 - 1);
		}
	}

	fclose(fp);
}
