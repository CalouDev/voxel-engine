#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <math.h>
#include <filesystem>
#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Attenuation {
    float range;
    float constant;
    float linear;
    float quadratic;
};

// Attenuation data from https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
// unused for now
constexpr std::array<Attenuation, 12> ATTENUATIONS = {{
    {7.0f, 1.0f, 0.7f, 1.8f},
    {13.0f, 1.0f, 0.35f, 0.44f},
    {20.0f, 1.0f, 0.22f, 0.20f},
    {32.0f, 1.0f, 0.14f, 0.07f},
    {50.0f, 1.0f, 0.09f, 0.032f},
    {65.0f, 1.0f, 0.07f, 0.017f},
    {100.0f, 1.0f, 0.045f, 0.0075f},
    {160.0f, 1.0f, 0.027f, 0.0028f},
    {200.0f, 1.0f, 0.022f, 0.0019f},
    {325.0f, 1.0f, 0.014f, 0.0007f},
    {600.0f, 1.0f, 0.007f, 0.0002f},
    {3250.0f, 1.0f, 0.0014f, 0.000007f}
}};

namespace WINDOW {
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
}

namespace SPACE {
    constexpr glm::vec3 VECTOR_UP(0.0f, 1.0f, 0.0f);
}

namespace PLAYER {
    constexpr glm::vec3 DEFAULT_POS(0.0f, 2.0f, 0.0f);
    constexpr float SPD = 8.0f;
}

constexpr FT_UInt FONT_SIZE = 18;
constexpr float LINE_SPACING = 1.5f; // Times font height

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

    namespace GUI {
        namespace CROSSHAIR {
            constexpr glm::vec2 SIDE(0.4f, 0.95f);
            constexpr float TEX_WIDTH = (float)TILE_SZ / TILESET_SZ;
            constexpr float TEX_HEIGHT = (float)TILE_SZ / TILESET_SZ;
            constexpr float WIDTH = 20.0f;
            constexpr float HEIGHT = 20.0f;
        }

        namespace INVENTORY {
            constexpr glm::vec2 SIDE(0.0f, 0.95f);
            constexpr float TEX_WIDTH = 0.4f;
            constexpr float TEX_HEIGHT = 0.05f;
            constexpr float WIDTH = 512.0f;
            constexpr float HEIGHT = 64.0f;
        }

        namespace INVENTORY_CASE_SELECTED {
            constexpr glm::vec2 SIDE(0.0f, 0.846875f);
            constexpr float TEX_WIDTH = ((float)TILE_SZ + 1.0f) / TILESET_SZ;
            constexpr float TEX_HEIGHT = ((float)TILE_SZ + 2.0f) / TILESET_SZ;
            constexpr float WIDTH = 68.0f;
            constexpr float HEIGHT = 68.0f;
        }
    }
}

#endif
