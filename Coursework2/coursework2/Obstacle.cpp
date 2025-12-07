#include "Obstacle.h"

Obstacle::Obstacle(const float* objVertices, size_t vertexCount, const unsigned int* objIndices, size_t indexCount, glm::vec3 position, float moveSpeed, float rotationAngle, float rotationSpeed, glm::vec3 rotationAxis, glm::vec3 scaleAmount)
	: vertexCount(vertexCount), indexCount(indexCount), position(position), moveSpeed(moveSpeed), rotationAngle(rotationAngle), rotationAxis(rotationAxis), rotationSpeed(rotationSpeed), scaleAmount(scaleAmount)
{
    // Assign buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount, objVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), objIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


}

 void Obstacle::draw() const
{
    // model

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

 void Obstacle::updatePosition(float deltaTime)
 {
     position.z += moveSpeed * deltaTime;
	 //rotationAngle += rotationSpeed * deltaTime; 
 }

 // Set model
 glm::mat4 Obstacle::getModel()
 {
     glm::mat4 model = glm::mat4(1.0f);
     model = glm::translate(model, position);
	 model = glm::rotate(model, rotationAngle, rotationAxis);
     model = scale(model, scaleAmount);
	 return model;

 }
