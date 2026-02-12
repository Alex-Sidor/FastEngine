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

	Vec3 movementVector = { sinf(transform.eulerAngles.y) * inputVector.z + cosf(transform.eulerAngles.y) * inputVector.x,
							inputVector.y,
							cosf(transform.eulerAngles.y) * inputVector.z - sinf(transform.eulerAngles.y) * inputVector.x
	};

	transform.position += movementVector * dt;

	float arrowInput = (float)targetScreen->inputHandler->keyPressed(GLFW_KEY_RIGHT) - (float)targetScreen->inputHandler->keyPressed(GLFW_KEY_LEFT);
	arrowInput *= dt;

	float mouseInput = (targetScreen->inputHandler->frameMouseDelta().x) * 0.001f;

	transform.eulerAngles += {0, mouseInput + arrowInput, 0};

	camera->transform = transform;
}
