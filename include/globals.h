#ifndef GLOBALS_H
#define GLOBALS_H

struct Coord2D {
    float x;
    float y;
    constexpr Coord2D(float x, float y) : x(x), y(y) {}
};

struct Coord3D {
    float x;
    float y;
    float z;
    constexpr Coord3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

namespace WINDOW {
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
}

namespace PLAYER {
    constexpr Coord3D DEFAULT_POS(0.0f, 2.0f, -3.0f);
}

#endif
