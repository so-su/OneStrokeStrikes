#include "Enemy.hpp"

Enemy::Enemy() {}

void Enemy::draw() const { body.draw(Point{400, 200}); }
