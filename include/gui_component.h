#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "globals.h"

class GuiComponent {
private:
    unsigned int VAO;
    unsigned int VBO;
    float vertices[24];
public:
    GuiComponent(glm::vec2 pos, glm::vec2 tex_coords, float width, float height);

    void use();
    void destroy();
};

#endif
