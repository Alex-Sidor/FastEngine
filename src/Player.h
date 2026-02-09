#include "Camera.h"
#include "Screen.h"
#include "Transform.h"

class Player {
public:
	Player(Camera* sceneCamera);

	update(Screen targetScreen);

private:

	Transform transform;

	Camera* camera = nullptr;
};