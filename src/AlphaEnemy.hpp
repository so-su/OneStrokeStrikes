#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"
#include "ProgressBar.hpp"

class AlphaEnemy : public Polyomino {
   public:
    AlphaEnemy();
    
    void initialize();
    
    void get_damaged(int32 damage);
    
    bool alive()const;
    
    int32 attack_value()const;
    
    void draw()const;

   private:
    // 属するポリオミノ（連結成分）の番号
    Grid<int32> connected_component_id;
    
    // 外周のパスの、各連結成分ごとの配列
    Array<Array<Point>> perimeters;
    
    // 名前
    String name;
};
