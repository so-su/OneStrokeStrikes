#pragma once
#include "Common.hpp"
#include "AttackShape.hpp"

class AttackRoulette {
public:
    AttackRoulette();
    
    void initialize();
    
    void draw()const;
    
    void draw_disk(const Circle&)const;
    
    void draw_large_disk()const;
    
    void draw_small_disk()const;
    
    void draw_icons()const;
    
    bool pressed()const;
    
    void update_value(int32 green,int32 red,int32 blue);
    
    void update_angles();
    
    void go_around(double speed);
    
private:
    std::array<AttackShape,3> attack_shapes;
    
    std::array<int32,3> attack_random_nums;
    
    std::array<int32,3> values;

    std::array<double,3> start_angles;
    
    std::array<double,3> angles;
    
    double triangle_angle;
    
    static constexpr Circle small_circle{1280,760,25};
    
    static constexpr Circle large_circle{700,500,150};
    
    static constexpr Triangle triangle {680,330,720,330,700,400};
};
