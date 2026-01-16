#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "camera.h"

class Player {
private:
    Camera camera;
    glm::vec3 pos;

public:
    Player(glm::vec3 _pos, Camera& _camera);

    void move(glm::vec3 vec);
};

#endif
