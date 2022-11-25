#include "AttackRoulette.hpp"


AttackRoulette::AttackRoulette(){
    initialize();
}

void AttackRoulette::initialize(){
    std::fill(begin(values),end(values),10);
    update_angles();
    triangle_angle=0.0;
    std::fill(begin(attack_random_nums),end(attack_random_nums),0);
    
    for(auto i:step(3)){
        if(RandomBool(0.3)){
            attack_random_nums[i]=Random(5,10);
        }
        else{
            attack_shapes[i].initialize();
        }
    }
}

void AttackRoulette::draw()const{
    draw_large_disk();
    draw_icons();
    triangle.rotatedAt(700,500,triangle_angle).draw(Color{255,153,0});
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
    for(auto i:step(3)){
        Point center=Vec2{700,425}.rotateAt(Point{700, 500},start_angles[i]+angles[i]/2).asPoint();
        if(attack_random_nums[i]>0){
            Rect{Arg::center=center,92,92}.drawFrame(0,2,Palette::White);
            FontAsset(U"Result")(U"{}"_fmt(attack_random_nums[i])).drawAt(center, Palette::White);
        }
        else{
            attack_shapes[i].draw(center);
        }
    }
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

void AttackRoulette::go_around(double speed){
    triangle_angle+=speed*Scene::DeltaTime();
    triangle_angle=std::fmod(triangle_angle,Math::TwoPi);
}
