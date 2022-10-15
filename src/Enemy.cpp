#include "Enemy.hpp"

Enemy::Enemy() :body(Size{5,5},30){}

void Enemy::draw() const {body.draw(Point{400, 250},Size{50,50}); }
