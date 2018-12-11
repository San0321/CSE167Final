#include "window.h"
#include "Camera.h"
#include "Sound.h"

#include "GLFWStarterProject\Sphere.h"

const char* window_title = "CSE 167 Final";
int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

glm::vec3 lastPos;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Shaders
GLint shaderProgram;
GLint skyboxShaderProgram;
GLint curveShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// shaders for skybox
#define SKYBOX_VERTEX_SHADER_PATH "../skybox_shader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skybox_shader.frag"

// shaders for curve points
#define CURVE_VERTEX_SHADER_PATH "../curve_shader.vert"
#define CURVE_FRAGMENT_SHADER_PATH "../curve_shader.frag"


#define BACKGROUND_MUSIC_PATH "puzzle.wav"
#define SOUND_EFFECT_PATH "test.wav"

// Default camera parameters
Camera* cam;

// Objects
Sphere* sphere;
Curve* curve;
Skybox * skybox;

Sound* sound;

// motion blur integers
int testi = 0;
int testn = 15;

// Curve ID
int curveID = 0;

// Distance Traveled per curveID
float distanceTravled = 0.0f;

// Boolean value for Travel
bool pause = true;

// Boolean Value for motion blur
bool isMotionBlur = false;

// Boolean value for loop
bool loopOn = true;

void Window::initialize_objects()
{
	cam = new Camera(glm::vec3(0.0f, 25.0f, 50.0f));

	sphere = new Sphere();
	curve = new Curve();
	skybox = new Skybox();

	sound = new Sound();

	sound->createSource("background", BACKGROUND_MUSIC_PATH, loopOn);
	sound->createSource("sound", SOUND_EFFECT_PATH, !loopOn);
	sound->playSource("background");


	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	curveShaderProgram = LoadShaders(CURVE_VERTEX_SHADER_PATH, CURVE_FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(sound);
	delete(skybox);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxShaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);
}

void Window::idle_callback()
{
	if (height > 0)
	{
		P = glm::perspective(cam->Zoom, (float)width / (float)height, 0.1f, 1000.0f);
		V = cam->GetViewMatrix();
	}

	if (!pause) {
	
		distanceTravled += (0.1f * deltaTime);
		if (distanceTravled > 1.0f) {
			distanceTravled = 0.0f;
			++curveID;
			if (curveID > NUMBER_OF_CURVE - 1) {
				curveID = 0;
			}
		}

		glm::vec3 cv = curve->GetPoint(curveID, distanceTravled);
		sphere->Translate(cv);
	}
}

void Window::display_callback(GLFWwindow* window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the sphere
	sphere->Draw(shaderProgram);

	// Render the skybox
	skybox->Draw(skyboxShaderProgram);

	// Render the curve
	curve->Draw(curveShaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	if (isMotionBlur) {

		if (testi == 0) {
			glAccum(GL_LOAD, 1.0 / testn);
		}
		else {
			glAccum(GL_ACCUM, 1.0 / testn);
		}

		testi++;

		if (testi >= testn) {
			testi = 0;
			glAccum(GL_RETURN, 1.0);
			glfwSwapBuffers(window);
		}
	}
	else {
		glfwSwapBuffers(window);
	}
	// Swap buffers
	
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_W)
		{
			cam->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
		}
		else if (key == GLFW_KEY_S)
		{
			cam->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);

		}
		else if (key == GLFW_KEY_A)
		{
			cam->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
		}
		else if (key == GLFW_KEY_D)
		{
			cam->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
		}
	}

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_LEFT)
		{
			curve->PrevControlPoint();
			int id = curve->GetControlPointID();

			fprintf(stderr, "Control ID: %d\n", id);
		}
		else if (key == GLFW_KEY_P)
		{
			pause = !pause;
		}
		else if (key == GLFW_KEY_RIGHT)
		{
			curve->NextControlPoint();
			int id = curve->GetControlPointID();
			fprintf(stderr, "Control ID: %d\n", id);
		}

		if (key == GLFW_KEY_X && mods == GLFW_MOD_SHIFT)
		{
			curve->MovePoint(glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else if ((mods == GLFW_MOD_SHIFT) && (key == GLFW_KEY_Y))
		{
			curve->MovePoint(glm::vec3(0.0f, -1.0f, 0.0f));
		}
		else if ((mods == GLFW_MOD_SHIFT) && (key == GLFW_KEY_Z))
		{
			curve->MovePoint(glm::vec3(0.0f, 0.0f, -1.0f));
		}
		else if (key == GLFW_KEY_X)
		{
			curve->MovePoint(glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (key == GLFW_KEY_Y)
		{
			curve->MovePoint(glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (key == GLFW_KEY_Z)
		{
			curve->MovePoint(glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (key == GLFW_KEY_M)
		{
			isMotionBlur = !isMotionBlur;
		}
		else if (key == GLFW_KEY_E)
		{
			sound->playSource("sound");
		}
	}
}

void Window::mouse_button_callback(GLFWwindow * window, int btn, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		lastPos = glm::vec3(xpos, ypos, 0.0f);

		// Selection
		//if (action ==  GLFW_MOUSE_BUTTON_RIGHT)
		//{
		//	bezier->selectionDraw();

		//	unsigned char pix[4];
		//	glReadPixels(xpos, height - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
		//	if (pix[1] + pix[2] + pix[3] == 0)
		//	{
		//		bezier->id = pix[0];
		//	}
		//	else 
		//	{
		//		bezier->id = -1;
		//	}
		//}
	}
}

void Window::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		GLfloat xoffset = xpos - lastPos.x;
		GLfloat yoffset = lastPos.y - ypos;

		lastPos.x = xpos;
		lastPos.y = ypos;

		cam->ProcessMouseMovement(xoffset, yoffset);

		//switch (button)
		//{
		//	case GLFW_MOUSE_BUTTON_LEFT:
		//	{
		//		cam.ProcessMouseMovement(xoffset, yoffset);
		//		break;
		//	}
		//	case GLFW_MOUSE_BUTTON_RIGHT:
		//	{
		//		xoffset *= 0.016;
		//		yoffset *= 0.016;

		//		glm::vec3 x = xoffset * cam.Right;
		//		glm::vec3 y = yoffset * cam.Up;
		//		bezier->updateControlPoint(x + y);
		//		break;
		//	}
		//}

		lastPos = glm::vec3(xpos, ypos, 0.0f);
	}
}

void Window::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	cam->ProcessMouseScroll(yoffset * 0.01f);
}
