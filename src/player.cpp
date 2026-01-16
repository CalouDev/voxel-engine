#include "../include/player.h"

Player::Player(glm::vec3 _pos, Camera& _camera) : pos(_pos), camera(_camera) {}

void Player::move(glm::vec3 vec) {
    pos += vec;
}
