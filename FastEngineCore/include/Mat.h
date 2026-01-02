#pragma once

#include "Vector.h"

#include <cstring>

class Mat3x3 {
public:
    Mat3x3(const float mat[3][3]);

    float m[3][3] = {};

};

namespace Mat {
    Vec3 multiplyMat3x3(const Vec3& v, const Mat3x3& M);
}