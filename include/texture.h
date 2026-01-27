#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"

constexpr int TILESET_SZ = 320;
constexpr int TILE_SZ = 16;

namespace TEX {
    namespace DIRT {
        constexpr glm::vec2 SIDE(0.0f, 0.0f);
        constexpr glm::vec2 TOP(0.05f, 0.0f);
        constexpr glm::vec2 BOT(0.1f, 0.0f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace WALL {
        constexpr glm::vec2 SIDE(0.0f, 0.05f);
        constexpr glm::vec2 TOP(0.05f, 0.05f);
        constexpr glm::vec2 BOT(0.01f, 0.05f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace GLASS {
        constexpr glm::vec2 SIDE(0.0f, 0.1f);
        constexpr glm::vec2 TOP(0.05f, 0.1f);
        constexpr glm::vec2 BOT(0.01f, 0.1f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace WOOD {
        constexpr glm::vec2 SIDE(0.0f, 0.25f);
        constexpr glm::vec2 TOP(0.05f, 0.25f);
        constexpr glm::vec2 BOT(0.05f, 0.25f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace DIRT_SNOW {
        constexpr glm::vec2 SIDE(0.0f, 0.2f);
        constexpr glm::vec2 TOP(0.05f, 0.2f);
        constexpr glm::vec2 BOT(0.1f, 0.2f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace LEAVES {
        constexpr glm::vec2 SIDE(0.0f, 0.3f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace GRASS {
        constexpr glm::vec2 SIDE(0.1f, 0.65f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace DAISY {
        constexpr glm::vec2 SIDE(0.05f, 0.6f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace ROSE {
        constexpr glm::vec2 SIDE(0.0f, 0.6f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = (float)TILE_SZ / TILESET_SZ;
    }

    namespace DOOR {
        constexpr glm::vec2 SIDE(0.0f, 0.5f);
        constexpr float WIDTH = (float)TILE_SZ / TILESET_SZ;
        constexpr float HEIGHT = 2 * (float)TILE_SZ / TILESET_SZ;
    }

    namespace DARK_SPECULAR {
        constexpr glm::vec2 POS(0.0f, 0.0f);
        constexpr float SZ = 0.1f;
    }
}

#endif
