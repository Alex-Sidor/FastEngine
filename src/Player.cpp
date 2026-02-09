#include <Player.h>

Player::Player(Camera* sceneCamera)
{
	camera = sceneCamera;
}

Player::update(Screen targetScreen)
{
	Vec3 inputVector = {
			targetScreen.inputHandler->keyPressed(GLFW_KEY_D) - targetScreen.inputHandler->keyPressed(GLFW_KEY_A),
			targetScreen.inputHandler->keyPressed(GLFW_KEY_E) - targetScreen.inputHandler->keyPressed(GLFW_KEY_Q),
			targetScreen.inputHandler->keyPressed(GLFW_KEY_W) - targetScreen.inputHandler->keyPressed(GLFW_KEY_S),
	}

	transform.position += inputVector * 0.01f;


	caera->transform.position = transform.position;
}
