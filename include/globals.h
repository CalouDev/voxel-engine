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
constexpr int TILE_RATIO = TILE_SZ / TILESET_SZ;  

struct GuiData {
    glm::vec2 pos;
    glm::vec2 sz;
    glm::vec2 tex_coord;
    glm::vec2 tex_sz;
};

constexpr std::array<GuiData, 3> GUI = {{
    // INVENTORY
    {
        glm::vec2(WINDOW::WIDTH/2.0f - 512.0f/2, 10.0f),
        glm::vec2(512.0, 64.0f),
        glm::vec2(0.0f, 0.95f),
        glm::vec2(0.4f, 0.05f)
    },

    // INENVTORY SELECTED CASE
    {
        glm::vec2(WINDOW::WIDTH/2.0f - 512.0f/2 - 2.0f, 7.0f),
        glm::vec2(68.0f, 68.0f),
        glm::vec2(0.0f, 0.846875f),
        glm::vec2(((float)TILE_SZ + 1.0f) / TILESET_SZ, ((float)TILE_SZ + 2.0f) / TILESET_SZ)
    },

    // CROSSHAIR
    {
        glm::vec2(WINDOW::WIDTH/2.0f - 20.0f/2, WINDOW::HEIGHT/2.0f + 20.0f/2),
        glm::vec2(20.0f, 20.0f),
        glm::vec2(0.4f, 0.95),
        glm::vec2(TILE_RATIO, TILE_RATIO)
    }
}};

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
