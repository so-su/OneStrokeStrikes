#pragma once
#include "Common.hpp"

class ProgressBar {
   public:
    ProgressBar(const Rect& outer_bar, Color color);

    // バーの更新
    void update(double ratio);

    // バーの描画
    void draw() const;

   private:
    // 外枠を含めた全体のRoundRect
    const RoundRect outer_bar;

    // 内部のRect
    const Rect inner_bar_full;

    // 実際に描画する内部のバー
    Optional<Polygon> inner_bar_polygon;

    // 実際に描画する内部のバーを得るために、共通部分を取るためのマスク
    const Polygon inner_bar_mask;

    // 内部のバーの色
    const Color color;

    // 内部のバーの割合（0.0〜1.0）
    double ratio{1.0};
};
