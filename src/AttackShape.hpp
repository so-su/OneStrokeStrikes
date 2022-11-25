#pragma once
#include "Common.hpp"

class AttackShape {
public:
    AttackShape();
    
    void initialize();
    
    void draw(Point center)const;
    
    Grid<bool> shape;
    
private:
    Array<Point> cell_pos;
};
