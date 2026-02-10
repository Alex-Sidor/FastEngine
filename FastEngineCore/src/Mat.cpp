#include "Mat.h"

#include <stdio.h>

void Mat3x3::copyDataInto(const float mat[3][3]) {
    std::memcpy(m, mat, sizeof(m));
}


Vec3 Mat::multiplyMat3x3(const Vec3 v, const Mat3x3 M) {
    Vec3 output = {};

    output.x += M.m[0][0] * v.x;
    output.y += M.m[1][0] * v.x;
    output.z += M.m[2][0] * v.x;

    output.x += M.m[0][1] * v.y;
    output.y += M.m[1][1] * v.y;
    output.z += M.m[2][1] * v.y;

    output.x += M.m[0][2] * v.z;
    output.y += M.m[1][2] * v.z;
    output.z += M.m[2][2] * v.z;

    return output;
}

Mat3x3 Mat::createMatrixFromEuler(Vec3 eulerAngles) {
    Vec3 sin = { sinf(eulerAngles.x),	sinf(eulerAngles.y),	sinf(eulerAngles.z) };
    Vec3 cos = { cosf(eulerAngles.x),	cosf(eulerAngles.y),	cosf(eulerAngles.z) };

    float newMatrix[3][3] = {
    { (cos.y * cos.z) + (sin.y * sin.x * sin.z),   (sin.z * cos.x),   (sin.y * -cos.z) + (cos.y * sin.x * sin.z) },
    { (cos.y * -sin.z) + (sin.y * sin.x * cos.z),  (cos.z * cos.x),   (sin.z * sin.y) + (cos.y * sin.x * cos.z)  },
    { (sin.y * cos.x),                             (-sin.x),          (cos.y * cos.x)                            }
    };

    Mat3x3 m;
    m.copyDataInto(newMatrix);

    return m;
}
