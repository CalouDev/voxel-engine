#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "camera.h"

class Player {
private:
    glm::vec3 pos;
    float spd;
    Camera camera;

public:
    Player(glm::vec3 _pos, float _spd, Camera& _camera);

    void move(glm::vec3 vec);

    float getSpd() const { return spd; }
};

#endif
