#include "Transform.h"

void Transform::updateMatrix()
{
	Vec3 sin = { sinf(eulerAngles.x),	sinf(eulerAngles.y),	sinf(eulerAngles.z) };
	Vec3 cos = { cosf(eulerAngles.x),	cosf(eulerAngles.y),	cosf(eulerAngles.z) };

	float newMatrix[3][3] = {
	{ (cos.y * cos.z) + (sin.y * sin.x * sin.z),   (cos.z * sin.y * sin.x) - (sin.y * sin.z),   (cos.x * sin.y) },
	{ (cos.x * sin.z),                             (cos.x * cos.z),                            (-sin.x)        },
	{ (cos.y * sin.x * sin.z) - (sin.y * cos.z),   (cos.y * sin.x * cos.z) + (sin.y * sin.z),   (cos.y * cos.x) }
	};

	rotationMatrix.copyDataInto(newMatrix);
}
