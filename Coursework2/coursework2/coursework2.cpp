#include <iostream>
//#include <GL/glew.h>

#include "GLAD/glad.h"
#include <GLFW/glfw3.h>
#include "Main.h"
#include "LoadShaders.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "FastNoiseLite.h" 

// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Model Shaders
#include <modelShaders/mesh.h>
#include <modelShaders/shader.h>
#include <modelShaders/model.h>
#include <modelShaders/shader_m.h>


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
float timeElapsed = 0.0f;
float gameScore = 0.0f;

const int maxAsteroids = 25;
vector<Obstacle> asteroids; 

float spawnRangeX = 2.0f; 
float spawnRangeY = 1.5f;
float asteroidSpeed = 2.0f;
float asteroidSpawnInterval = 2.0f;



void spawnAsteroid(Model* models, float spawnRangeX, float spawnRangeY, vector<Obstacle>& asteroids) {
	int modelIndex = rand() % 3;
	Model* model = &models[modelIndex];


	// Random position within spawn range
	float xPos = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * spawnRangeX;
	float yPos = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * spawnRangeY;

	// Random movement and rotation
	float rotationSpeed = 0.5f + static_cast<float>(rand() % 5) * 0.2f;
	vec3 rotationAxis = vec3(static_cast<float>(rand() % 100) / 100.0f, static_cast<float>(rand() % 100) / 100.0f, static_cast<float>(rand() % 100) / 100.0f);
	if (rand() % 2 == 0) rotationAxis = -rotationAxis; 

	asteroids.push_back(Obstacle(model, vec3(xPos, yPos, -20.0f), 0.0f, rotationSpeed, rotationAxis, vec3(1.0f)));
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

	// GLAD LOAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader modelShader("shaders/vertexShader.vert", "shaders/fragmentShader.frag");
	modelShader.use();
	Model Ship("textures/ship/playerShip.obj");
	Model asteroidModel("textures/Asteroids/Rocky_Asteroid_6.obj");
	Model signatureModel("textures/Signature/Signature.obj");
	Model planet1Model("textures/Planets/Planet1.obj");
	Model planet2Model("textures/Planets/Planet2.obj");
	Model planet3Model("textures/Planets/Planet3.obj");
	Model planet4Model("textures/Planets/Planet4.obj");

	Model asteroidModels[3] = {
		Model("textures/Asteroids/Rocky_Asteroid_4.obj"),
		Model("textures/Asteroids/Rocky_Asteroid_5.obj"),
		Model("textures/Asteroids/Rocky_Asteroid_6.obj")
	};
	

	// Set the viewport
	glViewport(0, 0, windowWidth, windowHeight);

	// Resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// PCG Starfield Noise
	FastNoiseLite starNoise;
	starNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	starNoise.SetFrequency(0.1f);
	int Seed = rand() % 100;
	starNoise.SetSeed(Seed);

	// Background plane
	float backgroundVertices[] = {
		-1.0f,  1.0f, -5.0f,   0.0f, 1.0f,
		-1.0f, -1.0f, -5.0f,   0.0f, 0.0f,
		 1.0f, -1.0f, -5.0f,   1.0f, 0.0f,
		 1.0f,  1.0f, -5.0f,   1.0f, 1.0f
	};

	// Borderr vertices
	float borderVertices[] = {
		-2.0f, -1.5f, -1.0f,
		2.0f, -1.5f, -1.0f,
		2.0f, 1.5f, -1.0f,
		-2.0f, 1.5f, -1.0f
	};


	GLuint borderVAO, borderVBO;
	glGenVertexArrays(1, &borderVAO);
	glGenBuffers(1, &borderVBO);

	glBindVertexArray(borderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, borderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(borderVertices), borderVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

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

	Player player("textures/ship/playerShip.obj", glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, glm::vec3(0.025f), 0.0f);


	float asteroidsSpawned = 0.5f;
	glEnable(GL_DEPTH_TEST);

	const int starCount = 250;
	vector<vec3> starPositions(starCount);

	// Star Spawning positions
	for (int i = 0; i < starCount; i++) {
		float noiseX = starNoise.GetNoise(i + 0.0f, i + 100.0f);
		float noiseY = starNoise.GetNoise(i + 100.0f, i + 0.0f);
		float spawnRangeX = 60.0f;
		float spawnRangeY = 40.0f;
		noiseX *= spawnRangeX;
		noiseY *= spawnRangeY;
		float zPos = -50.0f - static_cast<float>(rand() % 100); // Random 
		starPositions[i] = vec3(noiseX, noiseY, zPos);
	}

	// Asteroid Initial Spawns
	for (int i = 0; i < maxAsteroids; i++) {
		spawnAsteroid(asteroidModels, spawnRangeX, spawnRangeY, asteroids);

		// Spawn every 2
		float zPos = -2.0f * i; // for example: -0, -2, -4, -6...
		asteroids[i].setPosition(vec3(asteroids[i].getPosition().x, asteroids[i].getPosition().y, zPos));
	}

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
		glEnable(GL_CULL_FACE);
		glActiveTexture(GL_TEXTURE0);
		
		// Timing
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		timeElapsed += deltaTime;

		// Asterpod speed increase over time
		asteroidSpeed = 2.0f + (timeElapsed / 20.0f);

		// Game score
		gameScore = timeElapsed * asteroidSpeed;

		cout << "Score: " << static_cast<int>(gameScore) 
			<< " | Time Elapsed: " << static_cast<int>(timeElapsed) << "s   \r";
		
		// Input
		processUserInput(window, player, deltaTime);

		// cam follew player (TESTING)
		cameraPosition = vec3(player.getPosition().x, player.getPosition().y, cameraPosition.z);

		// Background
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f); 
		
		// Camera
		view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 150.0f);

		// Camera tilt based on mouse position
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalize mouse pos to screen size
		float normalX = (mouseX / windowWidth) * 2.0f - 1.0f;
		float normalY = (mouseY / windowHeight) * 2.0f - 1.0f;

		// Max camera tilt angle
		float maxAngle = radians(3.0f);

		// Limit to max movement angle
		float deltaX = normalX * maxAngle;
		float deltaY = normalY * maxAngle;

		// Smooth mouse movenet
		static float smoothedX = 0.0f;
		static float smoothedY = 0.0f;
		float smoothFactor = 0.12f;

		smoothedX = mix(smoothedX, deltaX, smoothFactor);
		smoothedY = mix(smoothedY, deltaY, smoothFactor);

		// Apply tilt
		mat4 cameraTilt = rotate(mat4(1.0f), smoothedX, vec3(0, 1, 0));
		cameraTilt = rotate(cameraTilt, smoothedY, vec3(1, 0, 0));

		// Set view 
		view = cameraTilt * lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

		// Game Boundary MVP
		modelShader.use();
		mat4 borderModel = mat4(1.0f);

		modelShader.setMat4("model", borderModel);
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setBool("isTextured", false);
		modelShader.setBool("lightEnabled", false);
		modelShader.setVec4("modelColor", vec4(0.4, 0.8, 1.0, 0.6));
		glBindVertexArray(borderVAO);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		glBindVertexArray(0);

		// Star spawning
		float starSpeed = 5.0f * asteroidSpeed; 
		for (int i = 0; i < starCount; i++) {
			starPositions[i].z += starSpeed * deltaTime; // move stars towards player

			// Reset star after passing camera
			if (starPositions[i].z > cameraPosition.z) {
				float noiseX = starNoise.GetNoise(i + 0.0f, i + 100.0f) * 2.0f;
				float noiseY = starNoise.GetNoise(i + 100.0f, i + 0.0f) * 1.5f;
				float spawnRangeX = 60.0f;
				float spawnRangeY = 40.0f;
				noiseX *= spawnRangeX;
				noiseY *= spawnRangeY;
				float zPos = -50.0f - static_cast<float>(rand() % 100); 
				starPositions[i] = vec3(noiseX, noiseY, zPos);
			}
		}
		// Draw as points
		modelShader.setVec4("modelColor", vec4(1.0));
		glPointSize(2.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS);

		for (const auto& star : starPositions) {
			glVertex3f(star.x, star.y, star.z);
		}
		glEnd();

		float lightGlow = 1.0f + 0.1f * (sin(timeElapsed));

		modelShader.setVec3("lightPos", vec3(-80.0f, 10.0f, -90.0f));
		modelShader.setVec3("lightColor", vec3(2.0f, 2.0f, 1.6f) * lightGlow);
		// SIGNATURE

		modelShader.use();

		mat4 sigModel = mat4(1.0f);
		sigModel = translate(sigModel, vec3(1.0f, -2.0f, -1.0f));
		
		sigModel = rotate(sigModel, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
		float signatureAngle = sin(timeElapsed * 2.0f);
		sigModel = rotate(sigModel, radians(signatureAngle), vec3(1.0f));
		sigModel = scale(sigModel, vec3(0.3f));

		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("model", sigModel);

		modelShader.setBool("isTextured", false);
		modelShader.setBool("lightEnabled", true);
		modelShader.setVec4("modelColor", glm::vec4(1.0));

		signatureModel.Draw(modelShader);
		



		// PLAYER
		modelShader.use();

		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("model", player.getModel());

		
		
		modelShader.setVec3("viewPos", cameraPosition);

		modelShader.setBool("isTextured", false);
		modelShader.setBool("lightEnabled", true);
		modelShader.setVec4("modelColor", vec4(1.0, 0.25, 0.0, 1.0)); // Orange color

		player.draw(modelShader);


		// OBSTACLE 
		// Draw asteroids
		for (size_t i = 0; i < asteroids.size(); i++) {
			asteroids[i].updatePosition(deltaTime, asteroidSpeed);
			modelShader.use();
			modelShader.setMat4("view", view);
			modelShader.setMat4("projection", projection);
			modelShader.setVec3("lightPos", vec3(2.0f * deltaTime)); // Animate light position
			modelShader.setBool("isTextured", true);
			modelShader.setMat4("model", asteroids[i].getModel());
			asteroids[i].draw(modelShader);

			// Reset if passing camera
			if (asteroids[i].getPosition().z > cameraPosition.z) {
				float xPos = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * spawnRangeX;
				float yPos = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * spawnRangeY;
				float zPos = -50.0f; 
				asteroids[i].setPosition(vec3(xPos, yPos, zPos));

			}

			// Collision detection
			if (playerObstacleCollision(player, asteroids[i]))
			{
				cout << endl << "You've crashed!" << endl;
				glfwSetWindowShouldClose(window, true);
			}
		}


		// PLANET 1
		modelShader.use();
		mat4 planetModel = mat4(1.0f);
		planetModel = translate(planetModel, vec3(-20.0f, 3.0f, -100.0f));
		planetModel = translate(planetModel, vec3(0.0f, 0.0f, 0.3f * timeElapsed));
		planetModel = rotate(planetModel, radians(timeElapsed * 5.0f), vec3(0.0f, 1.0f, 0.0f));
		planetModel = scale(planetModel, vec3(15.0f));

		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("model", planetModel);
		modelShader.setBool("isTextured", true);

		planet1Model.Draw(modelShader);

		// PLANET 2
		modelShader.use();
		mat4 planet2ModelMat = mat4(1.0f);
		planet2ModelMat = translate(planet2ModelMat, vec3(15.0f, -4.0f, -80.0f));
		planet2ModelMat = translate(planet2ModelMat, vec3(0.0f, 0.0f, 0.6f * timeElapsed));
		planet2ModelMat = rotate(planet2ModelMat, radians(25.0f), vec3(0.0f, 0.0f, 0.4f));
		planet2ModelMat = rotate(planet2ModelMat, radians(timeElapsed * 8.0f), vec3(0.0f, -1.0f, 0.0f));
		planet2ModelMat = scale(planet2ModelMat, vec3(8.0f));
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("model", planet2ModelMat);
		modelShader.setBool("isTextured", true);

		planet2Model.Draw(modelShader);

		// PLANET 3
		modelShader.use();
		mat4 planet3ModelMat = mat4(1.0f);
		planet3ModelMat = translate(planet3ModelMat, vec3(-80.0f, 10.0f, -90.0f));
		planet3ModelMat = translate(planet3ModelMat, vec3(0.0f, 0.0f, 0.01f * timeElapsed));
		planet3ModelMat = rotate(planet3ModelMat, radians(15.0f), vec3(0.4f, 0.0f, 0.0f));
		planet3ModelMat = rotate(planet3ModelMat, radians(timeElapsed * 1.0f), vec3(0.0f, 1.0f, 0.0f));
		planet3ModelMat = scale(planet3ModelMat, vec3(40.0f));
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("model", planet3ModelMat);
		modelShader.setBool("isTextured", true);
		planet3Model.Draw(modelShader);

		// PLANET 4
		modelShader.use();
		mat4 planet4ModelMat = mat4(1.0f);
		planet4ModelMat = translate(planet4ModelMat, vec3(-50.0f, 10.0f, -140.0f));
		planet4ModelMat = translate(planet4ModelMat, vec3(0.2f * timeElapsed, 0.0f, 0.002f * timeElapsed));
		planet4ModelMat = rotate(planet4ModelMat, radians(30.0f), vec3(0.0f, 0.4f, 0.0f));
		planet4ModelMat = rotate(planet4ModelMat, radians(timeElapsed * 2.0f), vec3(1.0f, 0.0f, 0.0f));
		planet4ModelMat = scale(planet4ModelMat, vec3(20.0f));
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("model", planet4ModelMat);
		modelShader.setBool("isTextured", true);
		planet4Model.Draw(modelShader);


		// Swap buffers and poll events
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Collision detection
bool playerObstacleCollision(const Player& player, const Obstacle& obstacle)
{
	vec3 playerPos = player.getPosition();
	vec3 obstaclePos = obstacle.getPosition();

	// World-space bounding box
	vec3 minBox, maxBox;
	obstacle.getCollisionBox(minBox, maxBox);

	// Player bounding box
	vec3 playerMin, playerMax;
	player.getCollisionBox(playerMin, playerMax);


	// Shrink collision box
	float scaleBox = 0.5f; 
	minBox += vec3(scaleBox);
	maxBox -= vec3(scaleBox);

	bool playerCollided =
		(playerMax.x > minBox.x && playerMin.x < maxBox.x) &&
		(playerMax.y > minBox.y && playerMin.y < maxBox.y) &&
		(playerMax.z > minBox.z && playerMin.z < maxBox.z);

	return playerCollided;
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
	vec3 moveDirection = vec3(0.0f);

	// Close window
	if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(WindowIn, true);

	// Player Movement
	// W
	if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
		moveDirection.y += (1.0f);
	// S
	if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
		moveDirection.y -= (1.0f);
	// A
	if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
		moveDirection.x -= (1.0f);
	// D
	if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
		moveDirection.x += (1.0f);

	player.updatePosition(moveDirection, deltaTime);
}


