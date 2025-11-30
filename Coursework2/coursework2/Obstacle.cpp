#include "Obstacle.h"

Obstacle::Obstacle(const float* objVertices, size_t vertexCount, const unsigned int* objIndices, size_t indexCount, glm::vec3 position, float moveSpeed) : indexCount(indexCount), position(position), moveSpeed(moveSpeed)
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
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

 void Obstacle::updatePosition(float deltaTime)
 {
     position.z += moveSpeed * deltaTime;
 }