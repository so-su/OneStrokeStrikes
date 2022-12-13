#include "SimpleButton.hpp"

SimpleButton::SimpleButton(const Rect& rect_,Color gauge_color_,int32 thickness_) :rect(rect_),top{rect.top()},right{rect.right()},bottom{rect.bottom()},left{rect.left()},full_len{(top.length()+right.length())*2},gauge_color(gauge_color_),thickness(thickness_),mouseover_transition{0.2s,0.1s}{
    
}

void SimpleButton::draw()const{
    rect.drawFrame(2,2,Palette::Dimgray);
    
    double gauge_len=full_len*mouseover_transition.value();
    
    if(top.length()<=gauge_len){
        top.draw(thickness,gauge_color);
        gauge_len-=top.length();
    }
    else{
        Line{top.begin,top.begin+Vec2{gauge_len,0}}.draw(thickness,gauge_color);
        return;
    }
    
    if(right.length()<=gauge_len){
        right.draw(thickness,gauge_color);
        gauge_len-=right.length();
    }
    else{
        Line{right.begin,right.begin+Vec2{0,gauge_len}}.draw(thickness,gauge_color);
        return;
    }
    
    if(bottom.length()<=gauge_len){
        bottom.draw(thickness,gauge_color);
        gauge_len-=bottom.length();
    }
    else{
        Line{bottom.begin,bottom.begin+Vec2{-gauge_len,0}}.draw(thickness,gauge_color);
        return;
    }
    
    if(left.length()<=gauge_len){
        left.draw(thickness,gauge_color);
        gauge_len-=left.length();
    }
    else{
        Line{left.begin,left.begin+Vec2{0,-gauge_len}}.draw(thickness,gauge_color);
        return;
    }
}

void SimpleButton::update(){
    mouseover_transition.update(rect.contains(Cursor::Pos()));
}

bool SimpleButton::down()const{
    return MouseL.down() and rect.contains(Cursor::Pos());
}

Point SimpleButton::center()const{
    return rect.center().asPoint();
}
