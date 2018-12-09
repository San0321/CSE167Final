#include "Skybox.h"
#include "Window.h"




Skybox::Skybox()
{
	fprintf(stderr, "Skybox Constructor\n");

	// load the image
	std::vector<std::string> faces =
	{
		"./Skybox/right.ppm",
		"./Skybox/left.ppm",
		"./Skybox/top.ppm",
		"./Skybox/bottom.ppm",
		"./Skybox/front.ppm",
		"./Skybox/back.ppm"
	};

	float skyboxVertices[] = {
		// positions          
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f
	};

	/**/
	//float skyboxVertices[] = {
	//	// positions          
	//	-100.0f,  100.0f, -100.0f,
	//	-100.0f, -100.0f, -100.0f,
	//	 100.0f, -100.0f, -100.0f,
	//	 100.0f, -100.0f, -100.0f,
	//	 100.0f,  100.0f, -100.0f,
	//	-100.0f,  100.0f, -100.0f,

	//	-100.0f, -100.0f,  100.0f,
	//	-100.0f, -100.0f, -100.0f,
	//	-100.0f,  100.0f, -100.0f,
	//	-100.0f,  100.0f, -100.0f,
	//	-100.0f,  100.0f,  100.0f,
	//	-100.0f, -100.0f,  100.0f,

	//	 100.0f, -100.0f, -100.0f,
	//	 100.0f, -100.0f,  100.0f,
	//	 100.0f,  100.0f,  100.0f,
	//	 100.0f,  100.0f,  100.0f,
	//	 100.0f,  100.0f, -100.0f,
	//	 100.0f, -100.0f, -100.0f,

	//	-100.0f, -100.0f,  100.0f,
	//	-100.0f,  100.0f,  100.0f,
	//	 100.0f,  100.0f,  100.0f,
	//	 100.0f,  100.0f,  100.0f,
	//	 100.0f, -100.0f,  100.0f,
	//	-100.0f, -100.0f,  100.0f,

	//	-100.0f,  100.0f, -100.0f,
	//	 100.0f,  100.0f, -100.0f,
	//	 100.0f,  100.0f,  100.0f,
	//	 100.0f,  100.0f,  100.0f,
	//	-100.0f,  100.0f,  100.0f,
	//	-100.0f,  100.0f, -100.0f,

	//	-100.0f, -100.0f, -100.0f,
	//	-100.0f, -100.0f,  100.0f,
	//	 100.0f, -100.0f, -100.0f,
	//	 100.0f, -100.0f, -100.0f,
	//	-100.0f, -100.0f,  100.0f,
	//	 100.0f, -100.0f,  100.0f
	//};

	//************************** Load Object **************************//

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices) * sizeof(float), skyboxVertices, GL_STATIC_DRAW); // size for the second one in bytes, third one pointer to the first element in the array 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	int twidth = 2048;
	int theight = 2048;

	cubemapTexture = loadCubemap(faces, twidth, theight);

}

Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
}

unsigned int Skybox::loadCubemap(std::vector<std::string> faces, int twidth, int theight)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = skyBoxLoadPPM(faces[i].c_str(), twidth, theight);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			//stbi_image_free(data);
		}
		else
		{
			fprintf(stderr, "Cubemap texture failed to load at path: %s", faces[i]);
			//std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			//stbi_image_free(data);
		}
	}
	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


void Skybox::Update()
{
}

void Skybox::Draw(GLuint skyboxShaderProgram)
{
	glDepthMask(GL_FALSE);
	glUseProgram(skyboxShaderProgram);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glm::mat4 vp = Window::P * glm::mat4(glm::mat3(Window::V));
	uWorldViewProjection = glGetUniformLocation(skyboxShaderProgram, "ViewProjection");

	glUniformMatrix4fv(uWorldViewProjection, 1, GL_FALSE, &vp[0][0]);

	// glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(skyboxVAO);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	//glDrawElements(GL_TRIANGLES, (GLsizei) this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned char* Skybox::skyBoxLoadPPM(const char* filename, int& width, int& height)
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
