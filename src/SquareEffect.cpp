#include "SquareEffect.hpp"

SquareEffect::SquareEffect(Vec2 pos, double base_hue)
    : pos{ pos }
{
    for (auto _ :step(8))
    {
        Square square{
            RandomVec2(Circle{30}),
            Random(-0.3, 0.1),
            Random(5.0, 30.0),
            HSV{ base_hue + Random(-30.0, 30.0) }
        };
        squares << square;
    }
}

bool SquareEffect::update(double t)
{
    for (const auto& square : squares)
    {
        const double t2 = (square.start_time + t);

        if (not InRange(t2, 0.0, 1.0))
        {
            continue;
        }

        const double easing = EaseOutExpo(t2);
        
        const Vec2 square_pos=pos + square.offset +square.offset * 2 * t;
        
        const int32 square_size = static_cast<int32>(easing * square.scale);
        
        Rect{Arg::center=square_pos.asPoint(),square_size,square_size }.rotatedAt(square_pos,square_pos.x*6_deg).draw(ColorF{ square.color, 0.15 })
                .drawFrame( 0.5 * (1.0 - easing) * square.scale, square.color);
    }

    return (t < 1.3);
}
