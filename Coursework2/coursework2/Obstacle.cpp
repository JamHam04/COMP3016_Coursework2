#include "Obstacle.h"

Obstacle::Obstacle(const std::string& path, glm::vec3 position, float moveSpeed, float rotationAngle, float rotationSpeed, glm::vec3 rotationAxis, glm::vec3 scaleAmount)
	: obstacleModel(path), position(position), moveSpeed(moveSpeed), rotationAngle(rotationAngle), rotationAxis(rotationAxis), rotationSpeed(rotationSpeed), scaleAmount(scaleAmount)
{
}


void Obstacle::draw(Shader& shader) 
{
    obstacleModel.Draw(shader);

}

 void Obstacle::updatePosition(float deltaTime)
 {
     position.z += moveSpeed * deltaTime;
	 //rotationAngle += rotationSpeed * deltaTime; 
 }

 // Set model
 glm::mat4 Obstacle::getModel() const
 {
     glm::mat4 model = glm::mat4(1.0f);
     model = glm::translate(model, position);
	 model = glm::rotate(model, rotationAngle, rotationAxis);
     model = scale(model, scaleAmount);
	 return model;

 }
