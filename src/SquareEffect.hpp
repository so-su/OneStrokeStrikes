#pragma once
#include"Common.hpp"

struct SquareEffect : IEffect
{
public:
    SquareEffect(Vec2 pos, double base_hue);

    bool update(double t) override;
    
private:
    struct Square
    {
        Vec2 offset;
        double start_time;
        double scale;
        ColorF color;
    };

    Vec2 pos;

    Array<Square> squares;
};
