#include "AlphaEnemy.hpp"

AlphaEnemy::AlphaEnemy():Polyomino(Size{10,10},Size{20,20},Point{500,50}),hp_bar{Rect{500,200,200,10}}{
    initialize();
}

void AlphaEnemy::initialize(){
    Polyomino::initialize(Size{7, 7}, 100, Cell::Black);
    
    max_hp=hp=300;
    attack=20;
    hp_bar.update(1.0);
}

void AlphaEnemy::get_damaged(int32 damage){
    hp=Max(hp-damage,0);
    hp_bar.update((double)hp/max_hp);
}

bool AlphaEnemy::alive()const{
    return hp>0;
}

int32 AlphaEnemy::attack_value()const{
    return attack;
}

void AlphaEnemy::draw()const {
    hp_bar.draw();
    Polyomino::draw();
}
