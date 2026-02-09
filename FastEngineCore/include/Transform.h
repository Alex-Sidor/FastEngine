#pragma once

#include "Mat.h"
#include "Vector.h"

#include <cmath>

class Transform {
public:	
	void updateMatrix();

	Vec3 eulerAngles = {0,0,0};
	Vec3 scale = { 1,1,1 };
	Vec3 position = { 0,0,0 };

	Mat3x3 rotationMatrix;
};