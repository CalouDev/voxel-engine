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
        constexpr glm::vec2 SIDE(0.0f, 0.5f);
        constexpr glm::vec2 TOP(0.1f, 0.5f);
        constexpr glm::vec2 BOT(0.1f, 0.5f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace GRASS { // rename to grass
        constexpr glm::vec2 SIDE(0.9f, 0.9f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace DAISY {
        constexpr glm::vec2 SIDE(0.8f, 0.9f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace ROSE {
        constexpr glm::vec2 SIDE(0.7f, 0.9f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.1f;
    }

    namespace DOOR {
        constexpr glm::vec2 SIDE(0.0f, 0.6f);
        constexpr float WIDTH = 0.1f;
        constexpr float HEIGHT = 0.2f;
    }

    namespace DARK_SPECULAR {
        constexpr glm::vec2 POS(0.0f, 0.0f);
        constexpr float SZ = 0.1f;
    }
}

#endif
