#pragma once

#include <GLAD/glad.h>	

#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "modelShaders/model.h"
#include "modelShaders/shader.h"
 
class Obstacle
{
private:
    
 
	glm::vec3 position;
	glm::vec3 rotationAxis;
	glm::vec3 scaleAmount;

	float moveSpeed;
	float rotationAngle;
	float rotationSpeed;

	glm::vec3 modelMin;
	glm::vec3 modelMax;



public:
    
    Obstacle(const std::string& path, glm::vec3 position, float moveSpeed, float rotationAngle, float rotationSpeed, glm::vec3 rotationAxis, glm::vec3 scaleAmount);
	void draw(Shader& shader);
	void updatePosition(float deltaTime);
    glm::vec3 getPosition() const { return position; }
	glm::mat4 getModel() const;
    glm::vec3 getScale() const { return scaleAmount; }
	Model obstacleModel;
	void createCollisionBox();
	void getCollisionBox(glm::vec3& minOut, glm::vec3& maxOut) const;
}; 