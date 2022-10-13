#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"

class AlphaEnemy {
   public:
    AlphaEnemy();
    
    void draw()const;
    
   private:
    Polyomino body;
    
    String name;
    
    int32 hp;
    
    int32 attack;
};
