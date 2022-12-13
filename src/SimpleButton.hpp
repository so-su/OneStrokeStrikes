#pragma once
#include "Common.hpp"

class SimpleButton{
public:
    SimpleButton(const Rect& rect_,Color gauge_color_,int32 thickness_);
    
    void draw()const;
    
    void update();
    
    bool down()const;
    
    Point center()const;

private:
    const Rect rect;
    
    const Line top;
    const Line right;
    const Line bottom;
    const Line left;
    
    const double full_len;
    
    const Color gauge_color;
    
    const int32 thickness;
    
    Transition mouseover_transition;
};
