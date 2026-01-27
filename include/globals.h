#ifndef GLOBALS_H
#define GLOBALS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

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

#endif
