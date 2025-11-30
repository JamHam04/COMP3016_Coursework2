#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Main.h"
#include "LoadShaders.h"
#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Obstacle.h"


using namespace std;
using namespace glm;

// Shader program
GLuint program;

// Matrices
mat4 view;
mat4 projection;
mat4 model;

// Camera parameters
vec3 cameraPosition = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);


// Player position
vec3 playerPosition = vec3(0.0f, 0.0f, 0.0f);

// Game timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

int main()
{
	// Window Size
	int windowWidth = 1920;
	int windowHeight = 1080;

	// Initialize GLFW
	glfwInit();

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Coursework 2", NULL, NULL);

	// Check if window creation was successful
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	// Bind OpenGl context to the window
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewInit();

	// Load shaders
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/vertexShader.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
		{ GL_NONE, NULL }
	};

	// Create shader program
	program = LoadShaders(shaders); 
	glUseProgram(program);

	// Set the viewport
	glViewport(0, 0, windowWidth, windowHeight);

	// Resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// Vertex and index data //
	
	// Rectangle 
	float vertices[] = { 
	// Width, Height, Depth
		-0.3f, -0.3f, -0.3f,
		 0.3f, -0.3f, -0.3f,
		 0.3f,  0.3f, -0.3f,
		-0.3f,  0.3f, -0.3f,

		 0.3f, -0.3f,  0.3f,
		-0.3f, -0.3f,  0.3f,
		-0.3f,  0.3f,  0.3f,
		 0.3f,  0.3f,  0.3f
	};

	unsigned int indices[] = { 
		// Back face
		0, 1, 2,
		2, 3, 0,
		// Front face
		4, 5, 6,
		6, 7, 4,
		// Left face
		5, 0, 3,
		3, 6, 5,
		// Right face
		1, 4, 7,
		7, 2, 1,
		// Bottom face
		5, 4, 1,
		1, 0, 5,
	};


	// Rectangle 2
	float objectVertices[] = {     
		0.3f, 0.3f, 0.0f,
		0.3f, -0.3f, 0.0f,
		-0.3f, -0.3f, 0.0f,
		-0.3f, 0.3f, 0.0f
	};

	unsigned int objectIndices[] = {  
		0, 1, 3, 
		1, 2, 3 
	};

	// Set Colour location
	GLint colourLocation = glGetUniformLocation(program, "colourIn");
	
	//createNewObject(objectVertices, sizeof(objectVertices), objectIndices, sizeof(objectIndices), 1, 1, 2);
	GLsizei indexCount = sizeof(indices) / sizeof(indices[0]);
	GLsizei rectangle2IndexCount = sizeof(objectIndices) / sizeof(objectIndices[0]);
	Obstacle rectangle(vertices, sizeof(vertices), indices, indexCount, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	Obstacle rectangle2(objectVertices, sizeof(objectVertices), objectIndices, rectangle2IndexCount, glm::vec3(0.0f, 0.0f, -5.0f), 1.0f);

	
	float objectZPosition = -5.0f;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{

		// Timing
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processUserInput(window);

		// Background
		glClearColor(0.0f, 0.1f, 0.5f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT); 

		// Model
		model = mat4(1.0f);
		model = translate(model, playerPosition); // Move to player position

		// Camera
		view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

		// MVP (vertex shader uniform)
		mat4 mvp = projection * view * model; 
		GLint mvpLocation = glGetUniformLocation(program, "mvpIn");
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));

		////Drawing
		glUniform4f(colourLocation, 1.0f, 0.25f, 0.0f, 1.0f);
		rectangle.draw();

		glUniform4f(colourLocation, 1.0f, 0.75f, 0.5f, 1.0f);

		//model = mat4(1.0f);
		//
		//objectZPosition += deltaTime;
		//model = translate(model, vec3(0.0f, 0.0f, objectZPosition)); 
		//mvp = projection * view * model;
		//glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));

		model = mat4(1.0f);
		rectangle2.updatePosition(deltaTime);
		model = translate(model, rectangle2.getPosition()); 
		mvp = projection * view * model;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));
		rectangle2.draw();
		if (rectangle2.getPosition().z > 3.0f)
		{
			rectangle2 = Obstacle(objectVertices, sizeof(objectVertices), objectIndices, rectangle2IndexCount, glm::vec3(0.0f, 0.0f, -5.0f), 1.0f); // Reset position
		}
		



		// Swap buffers and poll events
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Callback function called on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

// User inputs
void processUserInput(GLFWwindow* WindowIn)
{
	// Movement speed
	float movementSpeed = 1.0f * deltaTime; 

	// Boundary


	// Close window
	if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(WindowIn, true);

	// W
	if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
		playerPosition += vec3(0.0f, movementSpeed, 0.0f);
	// S
	if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
		playerPosition += vec3(0.0f, -movementSpeed, 0.0f);
	// A
	if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
		playerPosition += vec3(-movementSpeed, 0.0f, 0.0f);
	// D
	if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
		playerPosition += vec3(movementSpeed, 0.0f, 0.0f);
}

