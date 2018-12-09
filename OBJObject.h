#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>


class OBJObject
{
public:
	OBJObject(const char* objFilepath, const char* textureFilepath);
	~OBJObject();

	virtual void Update();
	virtual void Draw(glm::mat4 model, GLuint shaderProgram);

	unsigned int texture;
	GLuint VAO, VBO, TBO, NBO, EBO;

	GLuint uWorldViewProjection;
private:
	void parse(const char* filepath);
	unsigned char * loadPPM(const char* filename, int& width, int& height);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;


	//BoundingSphere mBoundingSphere;
	float mRadius;
};

#endif