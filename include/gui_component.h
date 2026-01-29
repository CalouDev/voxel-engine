#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "globals.h"

class GuiComponent {
private:
    float width, height;
    GLuint VAO;
    GLuint VBO;
    float vertices[24];
public:
    GuiComponent() = default;
    GuiComponent(glm::vec2 _pos, glm::vec2 tex_coords, glm::vec2 tex_sz, float width, float height);

    void move(glm::vec2 pos);
    void destroy();

    inline GLuint getVAO() const { return VAO; }
    void setPosition(glm::vec2 pos);
};

#endif
