#include "Transform.h"

void Transform::updateMatrix()
{
	Vec3 sin = { sinf(eulerAngles.x),	sinf(eulerAngles.y),	sinf(eulerAngles.z) };
	Vec3 cos = { cosf(eulerAngles.x),	cosf(eulerAngles.y),	cosf(eulerAngles.z) };

	float newMatrix[3][3] = {
	{ (cos.y * cos.z) + (sin.y * sin.x * sin.z),   (sin.z * cos.x),   (sin.y * -cos.z) + (cos.y * sin.x * sin.z) },
	{ (cos.y * -sin.z) + (sin.y * sin.x * cos.z),  (cos.z * cos.x),   (sin.z * sin.y) + (cos.y * sin.x * cos.z)  },
	{ (sin.y * cos.x),                             (-sin.x),          (cos.y * cos.x)                            }
	};

	rotationMatrix.copyDataInto(newMatrix);
}
