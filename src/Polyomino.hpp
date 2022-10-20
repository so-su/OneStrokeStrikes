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
    Polyomino(Size max_grid_size,Size cell_size, Point upper_left);

    // ポリオミノを描画する
    void draw() const;

    // パスを描画する
    void draw_path() const;

    // カーソルの座標を受け取ってパスを更新
    bool update_path(Point pos);

    // パスを削除
    void clear_path();
    
    // パスを逆順にする
    void reverse_path();

    // ポリオミノの消滅を進める
    bool vanish();

    // ポリオミノが消滅中かを返す
    bool is_vanishing() const;

    // セルが埋められているかを返す
    bool is_filled(int32 x, int32 y) const;

   protected:
    // セルの大きさ
    const Size cell_size;

    // ポリオミノの中心の座標
    const Point center;
    
    // ポリオミノの中心の座標
    Point upper_left;
    
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

    // ポリオミノが倒されて消滅していく最中のインデックス
    Optional<size_t> vanishing_idx = none;

    // bodyからシードを生成する
    int64 generate_seed() const;
    
    // ポリオミノの初期化
    void initialize(Size grid_size, int32 tolerance,Cell designated = Cell::None);

   private:
    void resize();
    
    static constexpr std::array<Point, 4> directions = {
        Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};

    // セルを生成する
    Cell generate_cell(Cell designated) const;
};
