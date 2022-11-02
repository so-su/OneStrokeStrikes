#include "ProgressBar.hpp"

ProgressBar::ProgressBar(Rect outer_bar,Color color):outer_bar(outer_bar.rounded(10)),inner_bar_full(outer_bar.stretched(-3)),inner_bar_mask(inner_bar_full.rounded(10).asPolygon()),color(color),ratio(1.0){
    
}

void ProgressBar::update(double ratio_){
    this->ratio=ratio_;
    
    Rect inner_bar_rect=inner_bar_full.stretched(0,-inner_bar_full.w*(1.0-ratio),0,0);
    if(auto polygons=Geometry2D::And(inner_bar_rect,inner_bar_mask);polygons.empty()){
        inner_bar_polygon=none;
    }
    else{
        inner_bar_polygon=polygons.front();
    }
}

void ProgressBar::draw()const {
    outer_bar.draw(ColorF{0.2});
    if(inner_bar_polygon.has_value()){
        inner_bar_polygon->draw(color);
    }
}
