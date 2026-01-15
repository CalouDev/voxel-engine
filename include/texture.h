#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"

namespace TEX {
    namespace DIRT {
        constexpr Coord2D SIDE(0.0f, 0.9f);
        constexpr Coord2D TOP(0.1f, 0.9f);
        constexpr Coord2D BOT(0.2f, 0.9f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace WALL {
        constexpr Coord2D SIDE(0.0f, 0.8f);
        constexpr Coord2D TOP(0.1f, 0.8f);
        constexpr Coord2D BOT(0.2f, 0.8f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace DOOR {
        constexpr Coord2D SIDE(0.0f, 0.6f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.2f;
    }
}

#endif
