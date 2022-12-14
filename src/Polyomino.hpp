#pragma once
#include "Common.hpp"
#include "RingEffect.hpp"
#include "SquareEffect.hpp"

// セルの情報をもつクラス
enum class Cell : uint8 {
    None,
    Red,
    Blue,
    Green,
    Yellow,
    Gray,
};

struct Score {
    int32 green;
    int32 red;
    int32 blue;
    int32 green_bonus;
    int32 red_bonus;
    int32 blue_bonus;
};

class Polyomino {
   public:
    Polyomino(Size max_grid_size, Size cell_size, Point center);

    // ポリオミノを描画する
    void draw() const;

    // パスを描画する
    void draw_path() const;

    // カーソルの座標を受け取ってパスを更新
    bool update_path(Point pos);

    // RingEffectを発生させる
    void add_ring_effect() const;

    // パスを削除
    void clear_path();

    // パスを逆順にする
    void reverse_path();

    // ポリオミノの消滅を進める
    bool vanish();

    // ポリオミノが消滅中かを返す
    bool is_vanishing() const;

    // ポリオミノが完全に消滅したかを返す
    bool has_vanished() const;

    // セルが埋められているかを返す
    bool is_filled(int32 x, int32 y) const;

    void draw_effect() const;

    Score get_ordinary_score() const;

    Score get_path_score() const;

    void prepare_to_randomly_vanish();

    // ポリオミノの左上の座標
    Point upper_left;

   protected:
    // セルの大きさ
    const Size cell_size;

    // ポリオミノの中心の座標
    const Point center;

    // グリッドの大きさ
    Size grid_size;

    // 有効なセルの個数
    size_t cell_num;

    // セル情報の二次元配列
    Grid<Cell> cells;

    // セルを表すRect
    Grid<Optional<Rect>> rects;

    // 一筆書き中のパス
    Array<Point> path;

    // 4方向
    static constexpr std::array<Point, 4> directions = {
        Point{1, 0}, Point{0, 1}, Point{-1, 0}, Point{0, -1}};

    // bodyからシードを生成する
    int64 generate_seed() const;

    // ポリオミノの初期化
    void initialize(Size grid_size, int32 tolerance,
                    Cell designated = Cell::None);

    Array<Point> shuffled_filled_cells;

   private:
    void resize();

    // セルを生成する
    Cell generate_cell(Cell designated) const;

    // ポリオミノが倒されて消滅していく最中のインデックス
    Optional<size_t> vanishing_idx = none;

    bool vanished;

    static constexpr Color path_color = MyColor::Yellow;

    Effect effect;
};
