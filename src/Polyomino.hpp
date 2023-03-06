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
};

struct Score {
    int32 green;
    int32 red;
    int32 blue;
    int32 green_endpoint;
    int32 red_endpoint;
    int32 blue_endpoint;
};

class Polyomino {
   public:
    Polyomino(Size max_grid_size, int32 cell_size, Point center);

    // ポリオミノを描画する
    void draw() const;
    
    // エフェクトを描画する
    void draw_effect() const;
    
    // ポリオミノの消滅を進め、消滅させるセルがなくなったらtrueを返す
    bool vanish();

    // ポリオミノが消滅中かを返す
    bool is_vanishing() const;

    // 埋まっているセルがあるかを返す
    bool is_alive() const;

    // セルが埋められているかを返す
    bool is_filled(int32 x, int32 y) const;
    
    // セルが埋められているかを返す
    bool is_filled(Point pos) const;
    
    // RingEffectを発生させる
    void add_ring_effect() const;

    // ポリオミノの左上の座標
    Point upper_left;

   protected:
    // ポリオミノを初期化する
    void initialize(Size grid_size, Cell designated = Cell::None);
    
    // easy modeかどうか
    bool easy_mode;
    
    // セルの大きさ
    const int32 cell_size;

    // ポリオミノの中心の座標
    const Point center;

    // グリッドの大きさ
    Size grid_size;

    // 埋まっているセルの個数
    size_t num_filled_cells;

    // セル情報の二次元配列
    Grid<Cell> cells;

    // セルを表すRect
    Grid<Optional<Rect>> rects;
    
    // 削除予定の埋まっているセルを入れるスタック
    Array<Point> cells_to_erase;

    // 埋まっているセルをシャッフルしてもっておく配列
    Array<Point> shuffled_filled_cells;

   private:
    // 一筆書き可能なポリオミノを生成する
    void generate_polyomino(Cell designated);
    
    // セルを生成する
    Cell generate_cell(Cell designated) const;
    
    // ポリオミノをグリッドの左上に寄せてgrid_sizeを詰める
    void shrink_to_fit();

    // エフェクトの管理
    Effect effect;
};
