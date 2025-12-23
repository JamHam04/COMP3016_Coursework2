#include "Player.h"
#include <glm/gtc/type_ptr.hpp>

Player::Player(const std::string& path, glm::vec3 startPos, float moveSpeed, glm::vec3 scale, float rotationAngle)
	: playerModel(path), position(startPos), moveSpeed(moveSpeed), scaleAmount(scale), rotationAngle(rotationAngle)
{
}

void Player::draw(Shader& shader)
{
    playerModel.Draw(shader);

}

float currentAngle = 0.0f;       
float currentPitch = 0.0f;


void Player::updatePosition(glm::vec3 moveDirection, float deltaTime)
{
    position += moveDirection * moveSpeed * deltaTime;

	rotationAngle = moveDirection.x * 20.0f; // 20- max angle
	

    float pitchAngle = moveDirection.y * 15.0f;

	if (position.x < -2.0f) position.x = -2.0f;
	if (position.x > 2.0f) position.x = 2.0f;
	if (position.y < -1.5f) position.y = -1.5f;
	if (position.y > 1.5f) position.y = 1.5f;
    
	
    if (moveDirection != glm::vec3(0.0f))
    {
        currentAngle += (rotationAngle - currentAngle) * deltaTime;
        currentPitch += (-pitchAngle - currentPitch) * deltaTime;
	}
    else
    {
		currentAngle += (-rotationAngle - currentAngle) * deltaTime;
        currentPitch += (pitchAngle - currentPitch) * deltaTime;
    }
    
}

glm::mat4 Player::getModel() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scaleAmount);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Face forward
	model = glm::rotate(model, glm::radians(currentAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(currentPitch), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    return model;
}

void Player::createCollisionBox()
{
    modelMin = glm::vec3(std::numeric_limits<float>::max());
    modelMax = glm::vec3(std::numeric_limits<float>::lowest());

    for (const auto& mesh : playerModel.meshes)
    {
        for (const auto& vertex : mesh.vertices)
        {
            modelMin.x = std::min(modelMin.x, vertex.Position.x);
            modelMin.y = std::min(modelMin.y, vertex.Position.y);
            modelMin.z = std::min(modelMin.z, vertex.Position.z);

            modelMax.x = std::max(modelMax.x, vertex.Position.x);
            modelMax.y = std::max(modelMax.y, vertex.Position.y);
            modelMax.z = std::max(modelMax.z, vertex.Position.z);
        }
    }

    modelCenter = (modelMin + modelMax) * 0.5f;
    modelHalfSize = (modelMax - modelMin) * 0.5f;
}


void Player::getCollisionBox(glm::vec3& minOut, glm::vec3& maxOut) const
{
    glm::mat4 modelMat = getModel();

    glm::vec3 minP(FLT_MAX);
    glm::vec3 maxP(-FLT_MAX);

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

    for (int i = 0; i < 8; ++i)
    {
        glm::vec4 worldV = modelMat * glm::vec4(corners[i], 1.0f);
        glm::vec3 p(worldV);

        minP = glm::min(minP, p);
        maxP = glm::max(maxP, p);
    }

    minOut = minP;
    maxOut = maxP;
}
