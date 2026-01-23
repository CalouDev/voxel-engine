#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"

namespace TEX {
    namespace DIRT {
        constexpr glm::vec2 SIDE(0.0f, 0.9f);
        constexpr glm::vec2 TOP(0.1f, 0.9f);
        constexpr glm::vec2 BOT(0.2f, 0.9f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace WALL {
        constexpr glm::vec2 SIDE(0.0f, 0.8f);
        constexpr glm::vec2 TOP(0.1f, 0.8f);
        constexpr glm::vec2 BOT(0.2f, 0.8f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace GLASS {
        constexpr glm::vec2 SIDE(0.0f, 0.4f);
        constexpr glm::vec2 TOP(0.0f, 0.04);
        constexpr glm::vec2 BOT(0.0f, 0.04);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace DOOR {
        constexpr glm::vec2 SIDE(0.0f, 0.6f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.2f;
    }
}

#endif
