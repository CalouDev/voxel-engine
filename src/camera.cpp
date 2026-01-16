#include "../include/camera.h"

Camera::Camera(glm::vec3 _pos, glm::vec3 _direction) : pos(_pos), direction(_direction) {
    right = glm::normalize(glm::cross(SPACE::VECTOR_UP, direction));
    up = glm::cross(direction, right);
}

void Camera::move(glm::vec3 vec) {
    pos += vec;
}
