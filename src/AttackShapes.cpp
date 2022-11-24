#include "AttackShapes.hpp"

AttackShapes::AttackShapes():shapes{3,{3,3,false}},shape_size{3,4,5,6,7,8}{
    for(auto i:step(3)){
        for(auto j:step(3)){
            cell_pos.emplace_back(i,j);
        }
    }
    
    initialize();
}

void AttackShapes::initialize(){
    for(auto& shape:shapes){
        shape.fill(false);
    }
    
    shape_size.shuffle();
    for(auto shape_idx:step(3)){
        cell_pos.shuffle();
        auto size=shape_size[shape_idx];
        for(auto pos_idx:step(size)){
            shapes[shape_idx][cell_pos[pos_idx]]=true;
        }
    }
}

void AttackShapes::draw()const{
    for(auto shape_idx:step(3)){
        for(auto i:step(3)){
            for(auto j:step(3)){
                Rect cell=Rect{1100+shape_idx*70+20*i,120+20*j,20,20};
                if(shapes[shape_idx][i][j]){
                    cell.draw(ColorF{0.9});
                }
                cell.drawFrame(1,1,ColorF{0.25});
            }
        }
    }
    Rect{1310,120,60,60}.drawFrame(1,1,ColorF{0.25});
}
