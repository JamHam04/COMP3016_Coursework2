#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "FastNoiseLite.h"

class Stars {
public:
    Stars(int starCount, int backgroundCount, float spawnRangeX, float spawnRangeY, FastNoiseLite& noise);

    void update(float deltaTime, float cameraZ, float speed);
    void draw() const;

private:
    std::vector<glm::vec3> stars;
    std::vector<glm::vec3> backgroundStars;
    float spawnRangeX, spawnRangeY;
    FastNoiseLite& noise;

    void spawnStars();
    void spawnBackgroundStars();
    void resetStar(glm::vec3& star) const;
};
