#pragma once
#include "Common.hpp"
#include "ProgressBar.hpp"

class Player{
public:
    Player();
    
    void get_damaged(int32 value);
    
    void get_healed(int32 value);
    
    void get_ap(int32 value);
    
    void get_sp(int32 value);
    
    bool alive()const;
    
    bool ap_is_full()const;
    
    bool sp_is_full()const;
    
    void reset_ap();
    
    void reset_sp();

    void draw()const;
    
private:
    int32 hp;
    
    int32 max_hp;
    
    int32 ap;
    
    int32 max_ap;
    
    int32 sp;
    
    int32 max_sp;

    ProgressBar hp_bar;
    
    ProgressBar ap_bar;
    
    ProgressBar sp_bar;
};
