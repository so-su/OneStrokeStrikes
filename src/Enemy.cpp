#include "Enemy.hpp"

Enemy::Enemy() : Polyomino(Size{10,10},Size{50, 50}, Point{400, 250}) {
    initialize();
}

void Enemy::initialize(){
    Polyomino::initialize(Size{6, 6}, 100);
}
