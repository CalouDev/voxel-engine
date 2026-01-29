#ifndef FLAT_VOXEL_H
#define FLAT_VOXEL_H

#include "globals.h"

class FlatVoxel {
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    float vertices[6 * 4];
    int indices[12];
public:
    FlatVoxel(glm::vec2 tex_coord_side, float width, float height);

    void use();
    void destroy();

    unsigned int getVAO() const { return VAO; }
};

#endif
