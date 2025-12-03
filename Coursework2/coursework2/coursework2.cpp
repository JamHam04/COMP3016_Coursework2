#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Main.h"
#include "LoadShaders.h"
#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Obstacle.h"
#include "Player.h"


using namespace std;
using namespace glm;

// Shader program
GLuint program;

// Matrices
mat4 view;
mat4 projection;
mat4 model;
mat4 mvp;

// Camera parameters
vec3 cameraPosition = vec3(0.0f, 0.0f, 1.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);


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
	float playerVertices[] = { 
	// Width, Height, Depth
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		-0.1f,  0.1f, -0.1f,

		 0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		 0.1f,  0.1f,  0.1f
	};

	unsigned int playerIndices[] = { 
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


	float objectVertices[] = {
		// Square obstacle border 
		 0.3f,  0.3f, 0.0f,  
		 0.3f, -0.3f, 0.0f, 
		-0.3f, -0.3f, 0.0f,  
		-0.3f,  0.3f, 0.0f, 

		// hole
		 0.2f,  0.1f, 0.0f, 
		 0.2f, -0.1f, 0.0f,  
		-0.2f, -0.1f, 0.0f,  
		-0.2f,  0.1f, 0.0f,  
	};

	unsigned int objectIndices[] = {
		0, 1, 5,  0, 5, 4,  
		1, 2, 6,  1, 6, 5,  
		2, 3, 7,  2, 7, 6,
		3, 0, 4,  3, 4, 7  
	};

	// Set locations
	GLint colourLocation = glGetUniformLocation(program, "colourIn");
	GLint mvpLocation = glGetUniformLocation(program, "mvpIn");

	GLsizei playerIndexCount = sizeof(playerIndices) / sizeof(playerIndices[0]);
	GLsizei obstacleIndexCount = sizeof(objectIndices) / sizeof(objectIndices[0]);
	Player player(playerVertices, sizeof(playerVertices), playerIndices, playerIndexCount, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, glm::vec3(0.5f)); // Change to player class 
	Obstacle obstacle(objectVertices, sizeof(objectVertices), objectIndices, obstacleIndexCount, glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, 0.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f));


	
	glEnable(GL_DEPTH_TEST);
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

		// Timing
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processUserInput(window, player, deltaTime);

		// Collision detection
		if (playerObstacleCollision(player, obstacle))
		{
			cout << "Player collided" << endl;
			break; 
		}

		// Background
		glClearColor(0.0f, 0.1f, 0.5f, 1.0f); 
		
		// Camera
		view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);



		// PLAYER MVP
		mvp = projection * view * player.getModel();;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));

		////Drawing
		glUniform4f(colourLocation, 1.0f, 0.25f, 0.0f, 1.0f);
		player.draw();


		// OBSTACLE MVP
		obstacle.updatePosition(deltaTime);
		mvp = projection * view * obstacle.getModel();
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));

		glUniform4f(colourLocation, 1.0f, 0.75f, 0.5f, 1.0f);
		obstacle.draw();

		// Reset obstacle position
		if (obstacle.getPosition().z > 3.0f)
		{
			obstacle = Obstacle(objectVertices, sizeof(objectVertices), objectIndices, obstacleIndexCount, glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f)); // Reset position
		}


		// Swap buffers and poll events
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Collision detection
bool playerObstacleCollision(Player player, Obstacle obstacle)
{
	vec3 playerPosition = player.getPosition();
	vec3 obstaclePosition = obstacle.getPosition();

	// Player and obstacle sizes
	float playerSize = 0.1f; // Half-size of player cube
	float obstacleSize = 0.3f; // Half-size of obstacle cube


	if (abs(playerPosition.x - obstaclePosition.x) < (playerSize + obstacleSize)&&
		abs(playerPosition.y - obstaclePosition.y) < (playerSize + obstacleSize)&&
		abs(playerPosition.z - obstaclePosition.z) < (playerSize + obstacleSize)) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Callback function called on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

// User inputs
void processUserInput(GLFWwindow* WindowIn, Player& player, float deltaTime)
{

	// Boundary


	// Close window
	if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(WindowIn, true);

	// W
	if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
		player.updatePosition(vec3(0.0f, 1.0f, 0.0f), deltaTime);
	// S
	if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
		player.updatePosition(vec3(0.0f, -1.0f, 0.0f), deltaTime);
	// A
	if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
		player.updatePosition(vec3(-1.0f, 0.0f, 0.0f), deltaTime);
	// D
	if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
		player.updatePosition(vec3(1.0f, 0.0f, 0.0f), deltaTime);
}

