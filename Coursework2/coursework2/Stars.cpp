#include "Stars.h"
#include <cstdlib>  // For rand()
#include "GLAD/glad.h" // or appropriate OpenGL header

Stars::Stars(int starCount, int backgroundCount, float spawnRangeX, float spawnRangeY, FastNoiseLite& noise)
    : stars(starCount), backgroundStars(backgroundCount),
    spawnRangeX(spawnRangeX), spawnRangeY(spawnRangeY), noise(noise)
{
    spawnStars();
    spawnBackgroundStars();
}

void Stars::update(float deltaTime, float cameraZ, float speed) {
    float starSpeed = speed;
    for (int i = 0; i < stars.size(); i++) {
        stars[i].z += starSpeed * deltaTime;
        if (stars[i].z > cameraZ) 
            resetStar(stars[i]);
    }
}

void Stars::draw() const {
    glPointSize(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (const auto& star : stars) glVertex3f(star.x, star.y, star.z);
    for (const auto& star : backgroundStars) glVertex3f(star.x, star.y, star.z);
    glEnd();
}

void Stars::spawnStars() {
    for (int i = 0; i < stars.size(); i++) {
        float noiseX = noise.GetNoise(i + 0.0f, i + 100.0f);
        float noiseY = noise.GetNoise(i + 100.0f, i + 0.0f);
        noiseX *= spawnRangeX;
        noiseY *= spawnRangeY;
        float zPos = -50.0f - static_cast<float>(rand() % 100); // Random
        stars[i] = glm::vec3(noiseX, noiseY, zPos);
    }
}

void Stars::spawnBackgroundStars() {
    for (int i = 0; i < backgroundStars.size(); i++) {
        float x = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * spawnRangeX;
        float y = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * spawnRangeY;
        float z = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 100.0f - 50.0f;
        backgroundStars[i] = glm::vec3(x, y, z);
    }
}

void Stars::resetStar(glm::vec3& star) const {
    float noiseX = noise.GetNoise(static_cast<float>(rand() % 100), static_cast<float>(rand() % 100)) * spawnRangeX;
    float noiseY = noise.GetNoise(static_cast<float>((rand() % 100) + 100), static_cast<float>(rand() % 100)) * spawnRangeY;
    float zPos = -50.0f - static_cast<float>(rand() % 100);
    star = glm::vec3(noiseX, noiseY, zPos);
}
