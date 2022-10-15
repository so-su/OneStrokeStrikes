#include "AlphaEnemy.hpp"

AlphaEnemy::AlphaEnemy():body(Size{7,7},100,Cell::Alpha){
    
}

void AlphaEnemy::draw()const{
    body.draw(Point{500,50},Size{20,20});
}
