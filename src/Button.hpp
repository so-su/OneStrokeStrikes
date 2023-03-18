#pragma once
#include "Common.hpp"

class Button {
   public:
    Button(const Array<Point>& path, Size grid_size_, int32 cell_size_,
           Point upper_left, Color color_, double alpha_min_);

    // ボタンの描画
    void draw() const;

    // ゲージの描画
    void draw_gauge() const;

    // ボタンの状態を更新し、ボタンが押されたかを返す
    // can_pressは、ボタンがまだ押されていないならば押されることを許容するか
    bool update(bool can_press);

    // 入力された座標がボタンと重なっているかを返す
    bool contains(Point pos) const;

    // 一筆書きが終了したかを返す
    bool completed() const;
    
    // ゲージのリセット
    void reset();

   private:
    // セルが埋まっているかを返す
    bool is_filled(int32 x, int32 y) const;
    
    // グリッドの大きさ
    const Size grid_size;

    // セルの一辺の長さ
    const int32 cell_size;

    // ボタンの色
    const Color color;
    
    // ボタンを押したときに一筆書きされるパス
    const Array<Point> path;

    // グリッドの各セルを表す正方形の配列
    Grid<Optional<Rect>> rects;
    
    // ボタンの外周を表すLineの配列
    Array<Line> perimeter;

    // ボタンの外周のゲージの進み具合
    Transition mouseover_transition{0.3s, 0.15s};
    
    // ボタンが押されたか
    bool pressed{false};
    
    // 一筆書きの始点となるセルのインデックス
    size_t start_index;

    // ボタンを押されてから一筆書きの進み具合
    Transition pressed_transition{0.8s, 0.4s};

    // ボタンの透過率の遷移
    Array<Transition> alpha_transitions;
    
    // ボタンの透過率の最小値
    const double alpha_min;
};
