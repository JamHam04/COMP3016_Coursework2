#pragma once

#include "modelShaders/model.h"


class Planet
{
private:
	Model planetModel;
	glm::vec3 position;
	glm::vec3 scaleAmount;
	glm::vec3 rotationAxis;
	float rotationAngle;
	float rotationSpeed;
	glm::vec3 drift;

public:
	Planet(Model planet, glm::vec3 startPos, glm::vec3 scale, glm::vec3 rotationAxis, float rotationSpeed, glm::vec3 drift);
	void draw(Shader& shader);
	void updatePosition(float deltaTime);
	glm::mat4 getModel() const;
};

