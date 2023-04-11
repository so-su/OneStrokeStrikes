#pragma once
#include "Common.hpp"

struct StringEffect : IEffect {
   public:
    StringEffect(const String& str, uint32 font_size, Point pos, double duration, double fadein_duration, double fadeout_duration);

    bool update(double time) override;

   private:
    // 表示する文字列
    const String str;
    
    // フォントサイズ
    const uint32 font_size;
    
    // エフェクトの中心座標
    const Point pos;
    
    // エフェクトの表示時間
    const double duration;
    
    // フェードイン時にエフェクトの透過率が変化する時間の長さ
    const double fadein_duration;
    
    // フェードアウト時にエフェクトの透過率が変化する時間の長さ
    const double fadeout_duration;
};
