#include "Camera.h"
#include "Screen.h"
#include "Transform.h"

class Player {
public:
	Player(Camera* sceneCamera);

	void update(Screen* targetScreen);

private:

	Transform transform;

	Camera* camera = nullptr;
};