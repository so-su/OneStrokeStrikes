#include "AttackRoulette.hpp"


AttackRoulette::AttackRoulette(){
    initialize();
}

void AttackRoulette::initialize(){
    std::fill(begin(values),end(values),10);
    update_angles();
}

void AttackRoulette::draw()const{
    draw_large_disk();
}

void AttackRoulette::draw_disk(const Circle& circle)const{
    circle
        .drawPie(start_angles[0], angles[0], Color{53, 161, 107})
        .drawPie(start_angles[1], angles[1], Color{255, 40, 0})
        .drawPie(start_angles[2], angles[2], Color{0, 65, 255})
        .drawFrame(circle.r*0.05,0,ColorF{0.9});
}

void AttackRoulette::draw_large_disk()const{
    draw_disk(large_circle);
}

void AttackRoulette::draw_small_disk()const{
    draw_disk(small_circle);
}

void AttackRoulette::draw_icons()const{
    
}

bool AttackRoulette::pressed()const{
    return MouseL.down() and small_circle.contains(Cursor::Pos());
}

void AttackRoulette::update_value(int32 green,int32 red,int32 blue){
    values[0]+=green;
    values[1]+=red;
    values[2]+=blue;
    update_angles();
}

void AttackRoulette::update_angles(){
    double sum=values[0]+values[1]+values[2];
    for(auto i:step(3)){
        angles[i]=Math::TwoPi*values[i]/sum;
    }
    
    start_angles[0]=0.0;
    start_angles[1]=angles[0];
    start_angles[2]=angles[0]+angles[1];
}

void AttackRoulette::go_around(){
}
