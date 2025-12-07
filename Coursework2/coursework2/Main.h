#pragma once

#include <GLFW/glfw3.h>
#include "Player.h"
#include "Obstacle.h"

// Called when the framebuffer window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Process user input 
void processUserInput(GLFWwindow* WindowIn, Player& player, float deltaTime);

bool playerObstacleCollision(Player player, Obstacle obstacle, float verticies[], size_t vertexCount);