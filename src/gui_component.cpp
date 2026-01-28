#include "../include/gui_component.h"

GuiComponent::GuiComponent(glm::vec2 pos, glm::vec2 tex_coords, float width, float height)
    : vertices{
        pos.x, pos.y, tex_coords.x, tex_coords.y,
        pos.x + width, pos.y, tex_coords.x + width, tex_coords.y,
        pos.x + width, pos.y, tex_coords.x + width, tex_coords.y + height,
        pos.x + width, pos.y, tex_coords.x + width, tex_coords.y + height,
        pos.x, pos.y + height, tex_coords.x, tex_coords.y + height,
        pos.x, pos.y, tex_coords.x, tex_coords.y
    }
{}

void GuiComponent::use() {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GuiComponent::destroy() {
    glDeleteVertexArrays(1, &VAO);
}
