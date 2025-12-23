#pragma once


#include <GLAD/glad.h>	

#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "modelShaders/model.h"
#include "modelShaders/shader.h"

class Player
{
private:
	Model playerModel;
	glm::vec3 position;
	float moveSpeed;
	glm::vec3 scaleAmount;
	float rotationAngle;


	glm::vec3 modelMin;
	glm::vec3 modelMax;
	glm::vec3 modelCenter;
	glm::vec3 modelHalfSize;


public:
	Player(const std::string& path, glm::vec3 startPos, float moveSpeed, glm::vec3 scale, float rotationAngle);
	void draw(Shader& shader);
	void updatePosition(glm::vec3 moveDirection, float deltaTime);
	glm::vec3 getPosition() const { return position; }
	glm::mat4 getModel() const;
	glm::vec3 getScale() const { return scaleAmount; }
	void createCollisionBox();
	void getCollisionBox(glm::vec3& minOut, glm::vec3& maxOut) const;
};

