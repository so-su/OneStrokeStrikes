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
    String name;
};
