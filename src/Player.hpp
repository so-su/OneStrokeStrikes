#pragma once
#include "Common.hpp"
#include "ProgressBar.hpp"

class Player{
public:
    Player();
    
    void get_damaged(int32 damage);
    
    bool alive()const;
    
    int32 attack_value()const;
    
    void draw()const;
    
private:
    int32 max_hp;
    
    int32 hp;
    
    int32 attack;
    
    int32 defence;
    
    ProgressBar hp_bar;
};
