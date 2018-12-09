#include "OBJObject.h"
#include "Window.h"

OBJObject::OBJObject(const char* objFilepath, const char* textureFilepath)
{
	fprintf(stderr, "OBJ Constructor\n");
	//************************** Load Object **************************//
	parse(objFilepath);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &TBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW); // size for the second one in bytes, third one pointer to the first element in the array 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//************************** Load Texture **************************//
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data
	
	// Load image file
	tdata = loadPPM(textureFilepath, twidth, theight);
	if (tdata==NULL) return;
	
	// Create ID for texture
	glGenTextures(1, &texture);

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//mBoundingSphere = new Sphere(mRadius);
}

OBJObject::~OBJObject()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &TBO);
	glDeleteBuffers(1, &EBO);
}


void OBJObject::Update()
{
}

void OBJObject::Draw(glm::mat4 model, GLuint shaderProgram)
{
	glUseProgram(shaderProgram);

	glm::mat4 wvp = Window::P * Window::V * model;
	uWorldViewProjection = glGetUniformLocation(shaderProgram, "WorldViewProjection");

	glUniformMatrix4fv(uWorldViewProjection, 1, GL_FALSE, &wvp[0][0]);

	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei) this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//mBoundingSphere->Draw(model, 0);
}

void OBJObject::parse(const char *filepath)
{
	FILE *fp;
	fp = fopen(filepath, "rb");

	if (fp == nullptr)
	{
		fprintf(stderr, "File Not Found\n");
		exit(-1);
	}

	char buf[64];

	float x, y, z;
	float nx, ny, nz;
	float u, v;
	std::vector<glm::vec3> vert;
	std::vector<glm::vec2> vertText;
	std::vector<glm::vec3> vertNorm;
	
	unsigned int v1,  v2,  v3;
	unsigned int vt1, vt2, vt3;
	unsigned int vn1, vn2, vn3;
	std::vector<unsigned int> vIndicies;
	std::vector<unsigned int> vtIndices;
	std::vector<unsigned int> vnIndices;

	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	minX = minY = minZ = INFINITY;
	maxX = maxY = maxZ = -INFINITY;

	// Read files
	while (fgets(buf, sizeof(buf), fp))
	{
		if (sscanf(buf, "v %f %f %f", &x, &y, &z))
		{
			// Read vertex
			vert.push_back(glm::vec3(x, y, z));

			// Find min of x, y, and z;
			minX = fmin(x, minX);
			minY = fmin(y, minY);
			minZ = fmin(z, minZ);

			// Find max of x, y, and z;
			maxX = fmax(x, maxX);
			maxY = fmax(y, maxY);
			maxZ = fmax(z, maxZ);
		}
		else if (sscanf(buf, "vn %f %f %f", &nx, &ny, &nz))
		{
			// Read vertex normal
			vertNorm.push_back(glm::vec3(nx, ny, nz));
		}
		else if (sscanf(buf, "vt %f %f", &u, &v))
		{
			// Read texture coordinate
			vertText.push_back(glm::vec2(u, v));
		}
		else if (sscanf(buf, "f %u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3))
		{
			// Read indices
			vIndicies.push_back(v1 - 1);
			vIndicies.push_back(v2 - 1);
			vIndicies.push_back(v3 - 1);

			vtIndices.push_back(vt1 - 1);
			vtIndices.push_back(vt2 - 1);
			vtIndices.push_back(vt3 - 1);

			vnIndices.push_back(vn1 - 1);
			vnIndices.push_back(vn2 - 1);
			vnIndices.push_back(vn3 - 1);
		}
	}

	fclose(fp);

	// Center of Objects
	float avgX = (minX + maxX) / 2.0f;
	float avgY = (minY + maxY) / 2.0f;
	float avgZ = (minZ + maxZ) / 2.0f;

	glm::vec3 maxVec = glm::vec3(0.0f, 0.0f, 0.0f);
	for (auto & a : vert)
	{
		// Centering objects
		a.x -= avgX;
		a.y -= avgY;
		a.z -= avgZ;
		
		// Find maximum radius
		if (glm::length(maxVec) < glm::length(a))
		{
			maxVec = a;
		}
	}

	mRadius = glm::length(maxVec);

	// Rerange and duplicates vertices, normals, and texture coordinates according to indicies
	for (unsigned int i = 0; i < vIndicies.size(); ++i)
	{
		this->vertices.push_back(vert[vIndicies[i]]);
		this->normals.push_back(vertNorm[vnIndices[i]]);
		this->uvs.push_back(vertText[vtIndices[i]]);

		this->indices.push_back(i);
	}
}

unsigned char* OBJObject::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	size_t read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ( (fp=fopen(filename, "rb")) == NULL)
	{
		fprintf(stderr, "error reading ppm file, could not locate %s\n", filename);
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width  = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
	  retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		fprintf(stderr, "error parsing ppm file, incomplete data\n");
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}
