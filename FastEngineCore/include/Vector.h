#pragma once

#include <cstring>

struct Vec3
{
    float x;
    float y;
    float z;
    
    Vec3() : x(0), y(0), z(0){}
    
    Vec3(float X, float Y, float Z){
        x = X;
        y = Y;
        z = Z;
    }

    Vec3 operator+(const Vec3& other) const{
        return Vec3(x + other.x,y + other.y,z + other.z);
    }

    Vec3 operator-(const Vec3& other) const{
        return Vec3(x - other.x,y - other.y, z - other.z);
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
};

struct Vec2
{
    float x;
    float y;
    
    Vec2() : x(0), y(0){}
    
    Vec2(float X, float Y){
        x = X;
        y = Y;
    }

    Vec2 operator+(const Vec2& other) const{
        return Vec2(x + other.x,y + other.y);
    }

    Vec2 operator-(const Vec2& other) const{
        return Vec2(x - other.x,y - other.y);
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};