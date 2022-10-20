#include "Enemy.hpp"

Enemy::Enemy() : Polyomino(Size{8,8},Size{50, 50}, Point{700, 500}) {
    initialize();
}

void Enemy::initialize(){
    Polyomino::initialize(Size{8, 8}, 100);
}
