#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"

class Enemy {
   public:
    Enemy();

    void draw() const;

   private:
    Polyomino body;
};
