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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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



	float depth = 0.5f;
	float objectVertices[] = {
		// FRONT FACE 
		 0.5f,  0.0f,       -depth ,  
		 0.25f, 0.4f,      -depth, 
		-0.25f, 0.4f,      -depth, 
		-0.5f,  0.0f,       -depth,   
		-0.25f,-0.4f,      -depth,   
		 0.25f,-0.4f,      -depth,

		 // BACK FACE
		  0.5f,  0.0f,        depth,  
		  0.2f, 0.4f,       depth, 
		 -0.2f, 0.4f,       depth,  
		 -0.5f,  0.0f,        depth, 
		 -0.2f,-0.4f,       depth,  
		  0.2f,-0.4f,       depth  
	};


	unsigned int objectIndices[] = {
		// FRONT FACE
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,

		// BACK FACE 
		6, 8, 7,
		6, 9, 8,
		6,10, 9,
		6,11,10,

		// SIDES
		0, 1, 7,   0, 7, 6,
		1, 2, 8,   1, 8, 7,
		2, 3, 9,   2, 9, 8,
		3, 4,10,   3,10, 9,
		4, 5,11,   4,11,10,
		5, 0, 6,   5, 6,11
	};

	// Background plane
	float backgroundVertices[] = {
		-1.0f,  1.0f, -5.0f,   0.0f, 1.0f,
		-1.0f, -1.0f, -5.0f,   0.0f, 0.0f,
		 1.0f, -1.0f, -5.0f,   1.0f, 0.0f,
		 1.0f,  1.0f, -5.0f,   1.0f, 1.0f
	};

	GLuint bgVAO, bgVBO;

	glGenVertexArrays(1, &bgVAO);
	glGenBuffers(1, &bgVBO);

	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Background 
	GLuint bgTexture;
	glGenTextures(1, &bgTexture);
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int width, height, colourChannels;
	unsigned char* data = stbi_load("textures/spaceBg.jpg", &width, &height, &colourChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
		return -1;
	}


	// Set locations
	GLint colourLocation = glGetUniformLocation(program, "colourIn");
	GLint mvpLocation = glGetUniformLocation(program, "mvpIn");

	size_t obstacleVertexCount = (sizeof(objectVertices) / sizeof(objectVertices[0]) / 3);
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

		// cam follew player (TESTING)
		cameraPosition = vec3(player.getPosition().x, player.getPosition().y, cameraPosition.z);


		// Collision detection
		if (playerObstacleCollision(player, obstacle, objectVertices, obstacleVertexCount))
		{
			cout << "Player collided" << endl;
			break; 
		}

		// Background
		//glClearColor(0.0f, 0.1f, 0.5f, 1.0f); 
		
		// Background MVP
		mat4 bgModel = mat4(1.0f); 
		bgModel = translate(bgModel, vec3(0.0f, 0.0f, -5.0f)); 
		bgModel = scale(bgModel, vec3(8.0f, 7.0f, 1.0f)); 
		mvp = projection * bgModel; 
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));

		// Draw background
		GLint useTextureLoc = glGetUniformLocation(program, "bgTexture");
		glUniform1i(useTextureLoc, 1);
		glBindTexture(GL_TEXTURE_2D, bgTexture);
		glBindVertexArray(bgVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


		
		// Camera
		view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);



		// PLAYER MVP
		mvp = projection * view * player.getModel();;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));

		////Drawing
		glUniform1i(useTextureLoc, 0);
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
			obstacle = Obstacle(objectVertices, sizeof(objectVertices), objectIndices, obstacleIndexCount, glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, 0.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f)); // Reset position
		}


		// Swap buffers and poll events
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	

	glfwTerminate();
	return 0;
}



// Collision detection
bool playerObstacleCollision(Player player, Obstacle obstacle, float verticies[], size_t vertexCount)
{
	vec3 playerPosition = player.getPosition();
	vec3 obstaclePosition = obstacle.getPosition();

	// Get obstacle model
	vec3 obstacleScale = obstacle.getScale();

	// Player and obstacle sizes
	vec3 playerSize = player.getScale() * 0.1f; 


	float minX = verticies[0] * obstacleScale.x + obstaclePosition.x; 
	float minY = verticies[1] * obstacleScale.y + obstaclePosition.y;
	float minZ = verticies[2] * obstacleScale.z + obstaclePosition.z;
	float maxX = verticies[0];
	float maxY = verticies[1];
	float maxZ = verticies[2];

	// Use vertices and indices
	for (size_t i = 1; i < vertexCount; i ++)
	{
		float vertexY = verticies[i * 3 + 1] * obstacleScale.y + obstaclePosition.y;
		float vertexZ = verticies[i * 3 + 2] * obstacleScale.z + obstaclePosition.z;

		if (vertexX < minX) minX = vertexX; // min x
		if (vertexY < minY) minY = vertexY; // min y
		if (vertexZ < minZ) minZ = vertexZ; // min z

		if (vertexX > maxX) maxX = vertexX; // max x
		if (vertexY > maxY) maxY = vertexY; // max y
		if (vertexZ > maxZ) maxZ = vertexZ; // max z

	}

	//std::cout << "Obstacle Min: (" << minX << ", " << minY << ", " << minZ << ")" << std::endl;
	//std::cout << "Obstacle Max: (" << maxX << ", " << maxY << ", " << maxZ << ")" << std::endl;

	// Check player and obstacle collision
		playerPosition.y + playerSize.y > minY && playerPosition.y - playerSize.y < maxY &&
		playerPosition.z + playerSize.z > minZ && playerPosition.z - playerSize.z < maxZ)
	{
		return true;
	}
	else {
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

