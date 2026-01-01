#pragma once

#include "Vector.h"

#include <cstring>

struct Mat3x3 {
    float m[3][3] = {};

    Mat3x3(const float mat[3][3]);
};

namespace Vector {
    Vec3 multiplyMat3x3(const Vec3& v, const Mat3x3& M);
}