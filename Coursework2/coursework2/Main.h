#pragma once

#include <GLFW/glfw3.h>

// Called when the framebuffer window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Process user input 
void processUserInput(GLFWwindow* WindowIn);
