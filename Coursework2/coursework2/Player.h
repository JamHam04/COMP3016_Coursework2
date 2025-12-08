#pragma once

#include <GL/glew.h>

#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/ext/matrix_transform.hpp"

class Player
{
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLsizei indexCount;
	glm::vec3 position;
	float moveSpeed;
	glm::vec3 scaleAmount;

public:
	Player(const float* objVertices, size_t vertexCount, const unsigned int* objIndices, size_t indexCount, glm::vec3 position, float moveSpeed, glm::vec3 scaleAmount);
	void draw() const;
	void updatePosition(glm::vec3 moveDirection, float deltaTime);
	glm::vec3 getPosition() const { return position; }
	glm::mat4 getModel();
	glm::vec3 getScale() const { return scaleAmount; }
};

