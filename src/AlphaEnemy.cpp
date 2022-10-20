#include "AlphaEnemy.hpp"

AlphaEnemy::AlphaEnemy():Polyomino(Size{8,20},Size{20,20},Point{500,50}){
    initialize();
}

void AlphaEnemy::initialize(){
    Polyomino::initialize(Size{20, 8}, 1000, Cell::Black);
}

bool AlphaEnemy::alive()const{
    return cell_num>0;
}

int32 AlphaEnemy::attack_value()const{
    return static_cast<int32>(cell_num);
}

void AlphaEnemy::draw()const {
    Polyomino::draw();
}
