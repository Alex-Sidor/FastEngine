#pragma once

#include <SDL3/SDL.h>
#include <stdexcept>

struct colour
{
    float r;
    float g;
    float b;
    
    colour() : r(0), g(0), b(0) {}

    colour(float R, float G, float B){
        r = R;
        g = G;
        b = B;
    }

    colour operator+(const colour& other) const {
        return colour(r + other.r, g + other.g, b + other.b);
    }

    colour& operator+=(const colour& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    colour operator-(const colour& other) const {
        return colour(r - other.r, g - other.g, b - other.b);
    }

    colour operator*(float scalar) const {
        return colour(r * scalar, g * scalar, b * scalar);
    }

    colour operator/(float scalar) const {
        return colour(r / scalar, g / scalar, b / scalar);
    }
};

class Screen {
public:
    Screen(int x, int y);
    void displayBuffer(Uint32* buffer);

    std::string error;

private:
    int width;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
};