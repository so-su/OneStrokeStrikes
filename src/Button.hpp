#pragma once
#include "Common.hpp"

class Button {
   public:
    Button(const Array<Point>& path, Size grid_size_, int32 cell_size_,
           Point upper_left, Color color_);

    // ボタンの描画
    void draw() const;

    // ゲージの描画
    void draw_gauge() const;

    // ボタンの状態を更新
    void update();

    // 入力された座標がボタンと重なっているかを返す
    bool contains(Point pos) const;

    // ゲージが満タンになっているかを返す
    bool completed() const;

   private:
    // セルが埋まっているかを返す
    bool is_filled(int32 x, int32 y) const;
    
    const Size grid_size;

    const int32 cell_size;

    const Color color;
    
    const Array<Point> path;

    Grid<Optional<Rect>> rects;
    
    Array<Line> perimeter;

    Transition mouseover_transition{0.2s, 0.1s};
    
    size_t start_index;

    Transition pressed_transition{0.8s, 0.4s};

    Array<Transition> alpha_transitions;
};
