#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"

class AlphaEnemy : public Polyomino {
   public:
    AlphaEnemy();

   private:
    String name;

    int32 hp;

    int32 attack;
};
