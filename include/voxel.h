#ifndef VOXEL_H
#define VOXEl_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "globals.h"

class Voxel {
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    float vertices[36 * 9];
    int indices[12];
public:
    Voxel(Coord2D tex_coord_side, Coord2D tex_coord_top, Coord2D tex_coord_bot, float width, float height);

    void use();
    void destroy();

    unsigned int getVAO() const { return VAO; }
};

#endif
