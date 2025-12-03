#pragma once

#include <GL/glew.h>

#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
 
class Obstacle
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLsizei indexCount;
	glm::vec3 position;
	float moveSpeed;

    // Rotation
	float rotationAngle;
	float rotationSpeed;
	glm::vec3 rotationAxis;
	glm::vec3 scaleAmount;


public:
    
    Obstacle(const float* objVertices, size_t vertexCount, const unsigned int* objIndices, size_t indexCount, glm::vec3 position, float moveSpeed, float rotationAngle, float rotationSpeed, glm::vec3 rotationAxis, glm::vec3 scaleAmount);
    void draw() const;
	void updatePosition(float deltaTime);
    glm::vec3 getPosition() const { return position; }
	glm::mat4 getModel();
}; 