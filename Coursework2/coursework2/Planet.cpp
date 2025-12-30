#include "Planet.h"

Planet::Planet(Model planet, glm::vec3 startPos, glm::vec3 scale, glm::vec3 rotationAxis, float rotationSpeed, glm::vec3 drift)
	: planetModel(planet), position(startPos), scaleAmount(scale), rotationAxis(rotationAxis), rotationAngle(0.0f), rotationSpeed(rotationSpeed), drift(drift)
{
}
void Planet::draw(Shader& shader) 
{
	shader.setMat4("model", getModel());
	planetModel.Draw(shader);
}
 void Planet::updatePosition(float timeElapsed)
 {
	 rotationAngle += rotationSpeed * timeElapsed;
	 position += drift * timeElapsed;
 }

 // Set model
 glm::mat4 Planet::getModel() const
 {
	 glm::mat4 model = glm::mat4(1.0f);
	 model = glm::translate(model, position);
	 model = glm::rotate(model, rotationAngle, rotationAxis);
	 model = scale(model, scaleAmount);
	 return model;

 }