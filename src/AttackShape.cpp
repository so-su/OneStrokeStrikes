#include "AttackShape.hpp"

AttackShape::AttackShape():shape{3,3,false}{
    for(auto i:step(3)){
        for(auto j:step(3)){
            cell_pos.emplace_back(i,j);
        }
    }
    
    initialize();
}

void AttackShape::initialize(){
    shape.fill(false);
    
    int32 size=Random(3,9);
    cell_pos.shuffle();
    for(auto pos_idx:step(size)){
        shape[cell_pos[pos_idx]]=true;
    }
}

void AttackShape::draw(Point center)const{
    for(auto i:step(3)){
        for(auto j:step(3)){
            if(shape[i][j]){
                Rect{Arg::center=center+Point{(i-1)*30,(j-1)*30},28,28}.draw(Palette::White);
            }
        }
    }
    Rect{Arg::center=center,92,92}.drawFrame(0,2,Palette::White);
}
