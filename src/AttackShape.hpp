#pragma once
#include "Common.hpp"

class AttackShape {
public:
    AttackShape();
    
    void initialize();
    
    void draw(Point center)const;
    
private:
    Grid<bool> shape;
    
    Array<Point> cell_pos;
};
