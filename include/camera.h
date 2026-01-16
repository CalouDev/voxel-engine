#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"

class Camera {
private:
    glm::vec3 pos;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;

public:
    Camera(glm::vec3 _pos, glm::vec3 _direction);

    void move(glm::vec3 vec);

    glm::vec3 getPos() const { return pos; }
    glm::vec3 getDirection() const { return direction; }
    glm::vec3 getUp() const { return up; }
    glm::vec3 getRight() const { return right; }

    void setPos(glm::vec3 _pos) { pos = _pos; }
    void setDirection(glm::vec3 _direction) { direction = _direction; }
    void setUp(glm::vec3 _up) { up = _up; }
    void setRight(glm::vec3 _right) { right = _right; }
};

#endif
