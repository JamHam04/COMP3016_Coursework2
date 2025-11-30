#pragma once

#include <GL/glew.h>

class Obstacle
{
private:
    
    GLuint VBO;
    GLuint EBO;

public:
    GLuint VAO;
    Obstacle(const float* objVertices, size_t vertexCount, const unsigned int* objIndices, size_t indexSize)
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, objIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);


    }

};