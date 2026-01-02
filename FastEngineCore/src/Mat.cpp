#include "Mat.h"

#include <stdio.h>

Mat3x3::Mat3x3(const float mat[3][3]) {
    std::memcpy(m, mat, sizeof(m));
}

Vec3 Mat::multiplyMat3x3(const Vec3 v, const Mat3x3 M) {
    Vec3 output = {};

    output.x += M.m[0][0] * v.x;
    output.x += M.m[0][1] * v.y;
    output.x += M.m[0][2] * v.z;

    output.y += M.m[1][0] * v.x;
    output.y += M.m[1][1] * v.y;
    output.y += M.m[1][2] * v.z;

    output.z += M.m[2][0] * v.x;
    output.z += M.m[2][1] * v.y;
    output.z += M.m[2][2] * v.z;
 

    return output;
}
