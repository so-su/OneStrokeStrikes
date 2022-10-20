#include "ProgressBar.hpp"

ProgressBar::ProgressBar(Rect outer_bar):outer_bar(outer_bar),inner_bar(outer_bar.stretched(-2)){
    
}

void ProgressBar::update(double ratio_){
    this->ratio=ratio_;
}

void ProgressBar::draw()const {
    outer_bar.draw(Palette::Black);
    inner_bar.stretched(0,-inner_bar.w*(1.0-ratio),0,0).draw(Palette::Green);
}
