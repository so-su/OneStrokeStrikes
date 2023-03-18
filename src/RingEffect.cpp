#include "RingEffect.hpp"

RingEffect::RingEffect(Vec2 pos, int32 size)
    : pos{pos}, color{RandomColorF()}, size(size) {}

bool RingEffect::update(double time) {
    // イージング関数の値
    const double easing = EaseOutExpo(time);

    Circle{pos, easing * size}.drawFrame(size * (1.0 - easing), color);

    return time < 1.0;
}
