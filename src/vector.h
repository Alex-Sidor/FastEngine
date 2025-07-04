#pragma once

struct vec3
{
    float x;
    float y;
    float z;
    
    vec3() : x(0), y(0), z(0){}
    
    vec3(float X, float Y, float Z){
        x = X;
        y = Y;
        z = Z;
    }

    vec3 operator+(const vec3& other) const{
        return vec3(x + other.x,y + other.y,z + other.z);
    }

    vec3 operator-(const vec3& other) const{
        return vec3(x - other.x,y - other.y, z - other.z);
    }

    vec3& operator+=(const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3 operator/(float scalar) const {
        return vec3(x / scalar, y / scalar, z / scalar);
    }

    vec3 operator*(float scalar) const {
        return vec3(x * scalar, y * scalar, z * scalar);
    }
};

struct vec2
{
    float x;
    float y;
    
    vec2() : x(0), y(0){}
    
    vec2(float X, float Y){
        x = X;
        y = Y;
    }

    vec2 operator+(const vec2& other) const{
        return vec2(x + other.x,y + other.y);
    }

    vec2 operator-(const vec2& other) const{
        return vec2(x - other.x,y - other.y);
    }

    vec2& operator+=(const vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};