#pragma once
#include "Common.hpp"
#include "ProgressBar.hpp"

class Player{
public:
    Player();
    
    void get_damaged(int32 value);
    
    void get_healed(int32 value);
    
    bool alive()const;

    void draw()const;
    
private:
    int32 hp;
    
    int32 max_hp;
    
    int32 ap;
    
    int32 max_ap;
    
    int32 sp;
    
    int32 max_sp;

    ProgressBar hp_bar;
};
