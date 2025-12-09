#include "Player.h"
#include <glm/gtc/type_ptr.hpp>

Player::Player(const std::string& path, glm::vec3 startPos, float moveSpeed, glm::vec3 scale, float rotationAngle)
	: shipModel(path), position(startPos), moveSpeed(moveSpeed), scaleAmount(scale), rotationAngle(rotationAngle)
{
}

void Player::draw(Shader& shader)
{
    //shader.setMat4("model", getModel());
    shipModel.Draw(shader);

}

float currentAngle = 0.0f;       


void Player::updatePosition(glm::vec3 moveDirection, float deltaTime)
{
    position += moveDirection * moveSpeed * deltaTime;

	rotationAngle = moveDirection.x * 20.0f; // 20- max angle
    
	
    if (moveDirection != glm::vec3(0.0f))
    {
        currentAngle += (rotationAngle - currentAngle) * deltaTime;
	}
    else
    {
		currentAngle += (-rotationAngle - currentAngle) * deltaTime;
    }
    
}

glm::mat4 Player::getModel() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scaleAmount);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Face forward
	model = glm::rotate(model, glm::radians(currentAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    return model;
}
