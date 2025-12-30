#pragma once
#include <GLAD/glad.h>	
#include <GLFW/glfw3.h>

#include "Player.h"
#include "Obstacle.h"
#include "Planet.h"
#include "Stars.h"

// Called when the framebuffer window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Process user input 
void processUserInput(GLFWwindow* WindowIn, Player& player, float deltaTime);

// Game update
void updateGame();
void updateCamera(const Player& player, GLFWwindow* window);

// Drawing
void drawGameBoundary(Shader& shader, GLuint borderVAO);
void drawSignature(Shader& shader, Model& signatureModel, float timeElapsed);

// Spawning
void spawnAsteroid(Model* models, float spawnRangeX, float spawnRangeY, std::vector<Obstacle>& asteroids);

// Collision detection
bool playerObstacleCollision(const Player& player, const Obstacle& obstacle);

