#pragma once
#include "Common.hpp"

class AttackShapes {
public:
    AttackShapes();
    
    void draw()const;
    
private:
    void initialize();
    
    Array<Grid<bool>> shapes;
    
    Array<int32> shape_size;
    
    Array<Point> cell_pos;
};
