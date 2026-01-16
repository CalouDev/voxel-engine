#include "../include/voxel.h"

Voxel::Voxel(glm::vec2 tex_coord_side, glm::vec2 tex_coord_top, glm::vec2 tex_coord_bot, float width, float height)
    : vertices{
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y + height,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y + height,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y + height,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y + height,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y + height,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y + height,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y,

        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y + height,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y + height,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y + height,

        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y + height,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y + height,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x, tex_coord_side.y,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_side.x + width, tex_coord_side.y + height,

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_bot.x, tex_coord_bot.y,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_bot.x + width, tex_coord_bot.y,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_bot.x + width, tex_coord_bot.y + height,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_bot.x + width, tex_coord_bot.y + height,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_bot.x, tex_coord_bot.y + height,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_bot.x, tex_coord_bot.y,

        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_top.x, tex_coord_top.y,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_top.x + width, tex_coord_top.y,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_top.x + width, tex_coord_top.y + height,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_top.x + width, tex_coord_top.y + height,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_top.x, tex_coord_top.y + height,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex_coord_top.x, tex_coord_top.y,
        // Mettre 1
    }
{}

void Voxel::use() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Voxel::destroy() {
    glDeleteVertexArrays(1, &VAO);
}
