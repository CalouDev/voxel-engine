#ifndef GLOBALS_H
#define GLOBALS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace WINDOW {
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
}

namespace SPACE {
    constexpr glm::vec3 VECTOR_UP(0.0f, 1.0f, 0.0f);
}

namespace PLAYER {
    constexpr glm::vec3 DEFAULT_POS(0.0f, 2.0f, -3.0f);
}

#endif
