#include "../include/player.h"

Player::Player(glm::vec3 _pos, float _spd, Camera& _camera) : pos(_pos), spd(_spd),camera(_camera) {}

void Player::move(glm::vec3 vec) {
    pos += vec;
}
