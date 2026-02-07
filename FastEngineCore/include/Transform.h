#pragma once

#include "Mat.h"
#include "Vector.h"

#include <cmath>

class Transform {
public:	
	void updateMatrix();

	Vec3 eulerAngles;
	Vec3 scale;
	Vec3 position;

	Mat3x3 rotationMatrix;
};