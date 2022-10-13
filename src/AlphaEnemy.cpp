#include "AlphaEnemy.hpp"

AlphaEnemy::AlphaEnemy():body(Size{7,7},100){
    
}

void AlphaEnemy::draw()const{
    body.draw(Point{400,0});
}
