#pragma once
#include "Common.hpp"

class ProgressBar {
   private:
    const RoundRect outer_bar;

    const Rect inner_bar_full;

    Optional<Polygon> inner_bar_polygon;

    const Polygon inner_bar_mask;

    const Color color;

    double ratio;

   public:
    ProgressBar(Rect outer_bar, Color color);

    void update(double ratio);

    void draw() const;
};
