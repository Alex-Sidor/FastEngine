#include <Player.h>

Player::Player(Camera* sceneCamera)
{
	camera = sceneCamera;
}

void Player::update(Screen* targetScreen)
{
	Vec3 inputVector = {
			(float)targetScreen->inputHandler->keyPressed(GLFW_KEY_D) - (float)targetScreen->inputHandler->keyPressed(GLFW_KEY_A),
			(float)targetScreen->inputHandler->keyPressed(GLFW_KEY_Q) - (float)targetScreen->inputHandler->keyPressed(GLFW_KEY_E),
			(float)targetScreen->inputHandler->keyPressed(GLFW_KEY_W) - (float)targetScreen->inputHandler->keyPressed(GLFW_KEY_S)
	};

	transform.position += inputVector;


	camera->transform.position = transform.position;
}
