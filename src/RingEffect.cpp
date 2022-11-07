#include "RingEffect.hpp"

RingEffect::RingEffect(Vec2 pos,int32 size)
    : pos{ pos }
    , color{ RandomColorF() },size(size) {}

bool RingEffect::update(double t)
{
    const double easing = EaseOutExpo(t);

    Circle{ pos, easing * size }.drawFrame(size*0.5 * (1.0 - easing), color);

    return t < 1.0;
}
