#pragma once
#include "Common.hpp"

struct RingEffect : IEffect
{
public:
    explicit RingEffect(Vec2 pos, int32 size);

    bool update(double t) override;

private:
    Vec2 pos;

    ColorF color;

    int32 size;
};
