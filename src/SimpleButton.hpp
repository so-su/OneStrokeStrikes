#pragma once
#include "Common.hpp"

class SimpleButton {
   public:
    SimpleButton(const Rect& rect_, Color inner_color_,Color gauge_color_, int32 thickness_);

    void draw() const;

    void update();

    bool down() const;

    Point center() const;
    
    void set_inner_color(Color color);

   private:
    const Rect rect;

    const Line top;
    const Line right;
    const Line bottom;
    const Line left;

    const double full_len;
    
    Color inner_color;

    const Color gauge_color;

    const int32 thickness;

    Transition mouseover_transition{0.2s, 0.1s};
};
