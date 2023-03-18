#pragma once
#include "Common.hpp"

struct RingEffect : IEffect {
   public:
    explicit RingEffect(Vec2 pos, int32 size);

    bool update(double time) override;

   private:
    // 中心の座標
    Vec2 pos;

    // 色
    ColorF color;

    // 大きさ
    int32 size;
};
