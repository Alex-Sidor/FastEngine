#pragma once

#include "Vector.h"

#include <cstring>
#include <cmath>

struct Mat3x3 {
    void copyDataInto(const float mat[3][3]);

    float m[3][3] = {};

};

namespace Mat {
    Vec3 multiplyMat3x3(const Vec3 v, const Mat3x3 M);

    Mat3x3 createMatrixFromEuler(Vec3 eulerAngles);
}