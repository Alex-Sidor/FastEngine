#include "Mat.h"

Mat3x3::Mat3x3(const float mat[3][3]) {
    std::memcpy(m, mat, sizeof(m));
}

Vec3 Mat::multiplyMat3x3(const Vec3& v, const Mat3x3& M) {
    Vec3 output{};

    for (int i = 0; i < 3; i++)
    {
        output.x += M.m[0][i] * (&v.x)[i];
        output.y += M.m[1][i] * (&v.y)[i];
        output.z += M.m[2][i] * (&v.z)[i];
    }

    return output;
}
