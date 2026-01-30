#include "../include/gui_component.h"

/*GuiComponent::GuiComponent(glm::vec2 pos, glm::vec2 tex_coords, glm::vec2 tex_sz, float width, float height)
    : width(width), height(height), vertices{
        pos.x, pos.y, tex_coords.x, tex_coords.y,
        pos.x + width, pos.y, tex_coords.x + tex_sz.x, tex_coords.y,
        pos.x + width, pos.y + height, tex_coords.x + tex_sz.x, tex_coords.y + tex_sz.y,
        pos.x + width, pos.y + height, tex_coords.x + tex_sz.x, tex_coords.y + tex_sz.y,
        pos.x, pos.y + height, tex_coords.x, tex_coords.y + tex_sz.y,
        pos.x, pos.y, tex_coords.x, tex_coords.y
    }
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}*/

GuiComponent::GuiComponent(const GuiData& data)
    : width(data.sz.x), height(data.sz.y), vertices{
        data.pos.x, data.pos.y, data.tex_coord.x, data.tex_coord.y,
        data.pos.x + data.sz.x, data.pos.y, data.tex_coord.x + data.tex_sz.x, data.tex_coord.y,
        data.pos.x + data.sz.x, data.pos.y + data.sz.y, data.tex_coord.x + data.tex_sz.x, data.tex_coord.y + data.tex_sz.y,
        data.pos.x + data.sz.x, data.pos.y + data.sz.y, data.tex_coord.x + data.tex_sz.x, data.tex_coord.y + data.tex_sz.y,
        data.pos.x, data.pos.y + data.sz.y, data.tex_coord.x, data.tex_coord.y + data.tex_sz.y,
        data.pos.x, data.pos.y, data.tex_coord.x, data.tex_coord.y
    }
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GuiComponent::destroy() {
    glDeleteVertexArrays(1, &VAO);
}

void GuiComponent::move(glm::vec2 pos) {
    for (int i = 0; i < 6; ++i) {
        vertices[i*4] += pos.x;
        vertices[i*4+1] += pos.y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
}

void GuiComponent::setPosition(glm::vec2 pos) {
    vertices[0] = pos.x; vertices[1] = pos.y;
    vertices[4] = pos.x + width; vertices[5] = pos.y;
    vertices[8] = pos.x + width; vertices[9] = pos.y + height;
    vertices[12] = pos.x + width; vertices[13] = pos.y + height;
    vertices[16] = pos.x; vertices[17] = pos.y + height;
    vertices[20] = pos.x; vertices[21] = pos.y;
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}
