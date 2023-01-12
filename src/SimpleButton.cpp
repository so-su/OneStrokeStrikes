#include "SimpleButton.hpp"

SimpleButton::SimpleButton(const Rect& rect_, Color inner_color_, Color gauge_color_,
                           int32 thickness_)
    : rect(rect_),
      top{rect.top()},
      right{rect.right()},
      bottom{rect.bottom()},
      left{rect.left()},
      full_len{(top.length() + right.length()) * 2},
      inner_color{inner_color_},
      gauge_color(gauge_color_),
      thickness(thickness_)
    {}

// ボタンの描画
void SimpleButton::draw() const {
    rect.draw(inner_color).drawFrame(2, 2, Palette::Dimgray);

    double gauge_len{full_len * mouseover_transition.value()};

    // 上の辺
    if (top.length() <= gauge_len) {
        top.draw(thickness, gauge_color);
        gauge_len -= top.length();
    } else {
        Line{top.begin, top.begin + Vec2{gauge_len, 0}}.draw(thickness,
                                                             gauge_color);
        return;
    }

    // 右の辺
    if (right.length() <= gauge_len) {
        right.draw(thickness, gauge_color);
        gauge_len -= right.length();
    } else {
        Line{right.begin, right.begin + Vec2{0, gauge_len}}.draw(thickness,
                                                                 gauge_color);
        return;
    }

    // 下の辺
    if (bottom.length() <= gauge_len) {
        bottom.draw(thickness, gauge_color);
        gauge_len -= bottom.length();
    } else {
        Line{bottom.begin, bottom.begin + Vec2{-gauge_len, 0}}.draw(
            thickness, gauge_color);
        return;
    }

    // 左の辺
    if (left.length() <= gauge_len) {
        left.draw(thickness, gauge_color);
        gauge_len -= left.length();
    } else {
        Line{left.begin, left.begin + Vec2{0, -gauge_len}}.draw(thickness,
                                                                gauge_color);
        return;
    }
}

// ボタンの状態を更新
void SimpleButton::update() {
    mouseover_transition.update(rect.contains(Cursor::Pos()));
}

// ボタンが押されたかを返す
bool SimpleButton::down() const {
    return MouseL.down() and rect.contains(Cursor::Pos());
}

// ボタンの中心座標を返す
Point SimpleButton::center() const { return rect.center().asPoint(); }

// ボタン内部の色を変更する
void SimpleButton::set_inner_color(Color color){
    inner_color=color;
}
