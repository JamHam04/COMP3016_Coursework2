#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Main.h"
#include "LoadShaders.h"
#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;


// Vertex Array Objects
enum VAO_IDs { Triangles, Indicies, NumVAOs = 4 }; 
GLuint VAOs[NumVAOs]; 

// Element Array Buffer
enum Buffer_IDs { ArrayBuffer, NumBuffers = 8 }; 
GLuint Buffers[NumBuffers];

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

void createNewObject(float* objVertices, size_t vertexCount, unsigned int* objIndices, size_t indexCount, GLuint vaoIndex)
{
	// VAO generation and binding
	glBindVertexArray(VAOs[vaoIndex]);

	// Bind vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount, objVertices, GL_STATIC_DRAW);
	// Bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indicies]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount, objIndices, GL_STATIC_DRAW);

	// Vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

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
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};

	unsigned int indices[] = { 
	0, 1, 3, 
	1, 2, 3 
	};

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

	// Set Colour
	GLint colourLocation = glGetUniformLocation(program, "colourIn");
	glUniform4f(colourLocation, 1.0f, 0.25f, 0.0f, 1.0f);


	// VAO generation and binding
	glGenVertexArrays(NumVAOs, VAOs);

	// Buffer generation
	glGenBuffers(NumBuffers, Buffers);

	// Create the rectangle object
	createNewObject(vertices, sizeof(vertices), indices, sizeof(indices), 0);
	// Create another object
	///createNewObject(objectVertices, 12, objectIndices, 1);


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

		//Drawing
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the rectangle (Triangles, 6 indices, type, offset 0)

		/*glBindVertexArray(VAOs[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/


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

