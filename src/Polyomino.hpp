#pragma once
#include "Common.hpp"

// セルの情報をもつクラス
enum class Cell : uint8 {
    None,
    Red,
    Blue,
    Green,
    Poison,
    Black,
};

class Polyomino {
   public:
    Polyomino(Size grid_size, Size cell_size, Point upper_left, int32 tolerance,
              Cell designated = Cell::None);

    // ポリオミノを描画する
    void draw() const;

    // パスを描画する
    void draw_path() const;

    // カーソルの座標を受け取ってパスを更新
    bool update_path(Point pos);

    // パスを削除
    void clear_path();

    // ポリオミノの消滅を進める
    bool vanish();

    // ポリオミノが消滅中かを返す
    bool is_vanishing() const;

    // セルが埋められているかを返す
    bool is_filled(int32 x, int32 y) const;

   protected:
    const Size grid_size;

    const Size cell_size;

    const Point upper_left;

    size_t cell_num = 1;

    Grid<Cell> cells;

    // セルを表すRect
    Grid<Optional<Rect>> rects;

    // 一筆書き中のパス
    Array<Point> path;

    // ポリオミノが倒されて消滅していく最中のインデックス
    Optional<size_t> vanishing_idx = std::nullopt;

    // bodyからシードを生成する
    int64 generate_seed() const;

   private:
    static constexpr std::array<Point, 4> directions = {
        Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};

    Cell random_cell(Cell designated) const;
};
