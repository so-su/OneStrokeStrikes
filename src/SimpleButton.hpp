#pragma once
#include "Common.hpp"

class SimpleButton {
   public:
    SimpleButton(const Rect& rect_, Color inner_color_,Color gauge_color_, int32 thickness_);

    // ボタンの描画
    void draw() const;

    // ボタンの状態を更新し、ボタンが押されたかを返す
    // can_pressは、ボタンがまだ押されていないならば押されることを許容するか
    bool update(bool can_press);
    
    // ボタンの状態をリセットする
    void reset();

    // ボタンが押されたかを返す
    bool down() const;
    
    // ボタンが押されていて、かつゲージが満タンの状態かを返す
    bool completed() const;

    // ボタンの中心座標を返す
    Point center() const;
    
    // ボタン内部の色を変更する
    void set_inner_color(Color color);

   private:
    // ボタン本体
    const Rect rect;

    // ボタン本体の各辺
    const Line top;
    const Line right;
    const Line bottom;
    const Line left;

    // 外周の全長
    const double full_len;

    // 外周の色
    const Color gauge_color;

    // 外周の太さ
    const int32 thickness;
    
    // ボタンの中身の色
    Color inner_color;

    // 外周のゲージの進み具合
    Transition gauge_transition{0.2s, 0.1s};
    
    // ボタンが押されたか
    bool pressed{false};
};
