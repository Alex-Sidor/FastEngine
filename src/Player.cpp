#include <Player.h>

Player::Player(Camera* sceneCamera)
{
	camera = sceneCamera;
}

void Player::update(Screen* targetScreen, float dt)
{
	Vec3 inputVector = {
			(float)targetScreen->inputHandler->keyPressed(GLFW_KEY_D) - (float)targetScreen->inputHandler->keyPressed(GLFW_KEY_A),
			(float)targetScreen->inputHandler->keyPressed(GLFW_KEY_Q) - (float)targetScreen->inputHandler->keyPressed(GLFW_KEY_E),
			(float)targetScreen->inputHandler->keyPressed(GLFW_KEY_W) - (float)targetScreen->inputHandler->keyPressed(GLFW_KEY_S)
	};

	transform.position += inputVector * dt;


	camera->transform.position = transform.position;
	camera->transform.eulerAngles += {0, targetScreen->inputHandler->frameMouseDelta().x * 0.01f, 0};
}
