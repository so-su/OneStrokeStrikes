#pragma once
#include"Common.hpp"

class ProgressBar{
private:
    Rect outer_bar;
    
    Rect inner_bar;
    
    Color color;
    
    double ratio=1.0;
public:
    ProgressBar(Rect outer_bar,Color color);
    
    void update(double ratio);
    
    void draw ()const;
};
