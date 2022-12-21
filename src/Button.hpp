#pragma once
#include "Common.hpp"

class Button {
   public:
    Button(const Array<Point>& path, Size grid_size_, int32 cell_size_,
           Point upper_left, Color color_);

    void draw() const;

    void draw_gauge() const;

    void update();

    bool contains(Point pos) const;

    bool completed() const;

   private:
    Array<Point> path;

    Grid<Optional<Rect>> rects;

    const Size grid_size;

    const int32 cell_size;

    const Color color;

    Transition mouseover_transition;

    Transition pressed_transition;

    Array<Transition> alpha_transitions;

    // 外周のパス
    Array<Line> perimeter;

    size_t start_index;

    bool is_filled(int32 x, int32 y) const;
};
