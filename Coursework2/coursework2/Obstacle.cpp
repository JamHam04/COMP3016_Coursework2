#include "Obstacle.h"

Obstacle::Obstacle(const std::string& path, glm::vec3 position, float moveSpeed, float rotationAngle, float rotationSpeed, glm::vec3 rotationAxis, glm::vec3 scaleAmount)
	: obstacleModel(path), position(position), moveSpeed(moveSpeed), rotationAngle(rotationAngle), rotationAxis(rotationAxis), rotationSpeed(rotationSpeed), scaleAmount(scaleAmount)
{
	createAABB();
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

 void Obstacle::createCollisionBox()
 {
     // Initialize min/max values
     modelMin = glm::vec3(std::numeric_limits<float>::max());
     modelMax = glm::vec3(std::numeric_limits<float>::lowest());
     
     // Iterate through all vertices of the obstacle model
     for (const auto& mesh : obstacleModel.meshes)
     {
         for (const auto& vertex : mesh.vertices)
         {
             // Add to min/max
             modelMin.x = std::min(modelMin.x, vertex.Position.x);
             modelMin.y = std::min(modelMin.y, vertex.Position.y);
             modelMin.z = std::min(modelMin.z, vertex.Position.z);
             modelMax.x = std::max(modelMax.x, vertex.Position.x);
             modelMax.y = std::max(modelMax.y, vertex.Position.y);
             modelMax.z = std::max(modelMax.z, vertex.Position.z);
         }
     }
 }

 void Obstacle::getCollisionBox(glm::vec3& minOut, glm::vec3& maxOut) const
 {
	 // Get model 
     glm::mat4 modelMat = getModel();

	 // Create min/max points
     glm::vec3 minP(FLT_MAX);
     glm::vec3 maxP(-FLT_MAX);

	 // Set the 8 corners of the collision box
     glm::vec3 corners[8] = {
         {modelMin.x, modelMin.y, modelMin.z},
         {modelMax.x, modelMin.y, modelMin.z},
         {modelMin.x, modelMax.y, modelMin.z},
         {modelMax.x, modelMax.y, modelMin.z},
         {modelMin.x, modelMin.y, modelMax.z},
         {modelMax.x, modelMin.y, modelMax.z},
         {modelMin.x, modelMax.y, modelMax.z},
         {modelMax.x, modelMax.y, modelMax.z}
     };

	 // Transform each corner to world space and update min/max
     for (int i = 0; i < 8; i++)
     {
         glm::vec4 worldV = modelMat * glm::vec4(corners[i], 1.0f);
         glm::vec3 p(worldV);

         minP = glm::min(minP, p);
         maxP = glm::max(maxP, p);
     }

	 // Set output
     minOut = minP;
     maxOut = maxP;
 }
