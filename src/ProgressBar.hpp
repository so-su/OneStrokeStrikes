#pragma once
#include"Common.hpp"

class ProgressBar{
private:
    Rect outer_bar;
    
    Rect inner_bar;
    
    double ratio=1.0;
public:
    ProgressBar(Rect outer_bar);
    
    void update(double ratio);
    
    void draw ()const;
};
