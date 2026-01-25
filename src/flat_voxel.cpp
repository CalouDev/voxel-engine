#include "../include/flat_voxel.h"

// Add EBO to FlatVoxel (topright, botleft meeting points)

FlatVoxel::FlatVoxel(glm::vec2 tex_coord_side, float width, float height)
    : vertices{
        -0.5f, -0.5f, tex_coord_side.x, tex_coord_side.y,
        0.5f, -0.5f, tex_coord_side.x + width, tex_coord_side.y,
        0.5f, 0.5f, tex_coord_side.x + width, tex_coord_side.y + height,
        0.5f, 0.5f, tex_coord_side.x + width, tex_coord_side.y + height,
        -0.5f, 0.5f, tex_coord_side.x, tex_coord_side.y + height,
        -0.5f, -0.5f, tex_coord_side.x, tex_coord_side.y,
    }
{}

void FlatVoxel::use() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void FlatVoxel::destroy() {
    glDeleteVertexArrays(1, &VAO);
}
