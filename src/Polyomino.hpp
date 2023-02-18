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

    // パスを描画する
    void draw_path() const;
    
    // エフェクトを描画する
    void draw_effect() const;

    // カーソルの座標を受け取ってパスを更新
    void update_path(Point cursor_pos);
    
    // パスを削除する
    void clear_path();
    
    // パスを反転させる
    void reverse_path();
    
    // ポリオミノの消滅を進め、すべて消滅したらtrueを返す
    bool vanish();

    // ポリオミノが消滅中かを返す
    bool is_vanishing() const;

    // ポリオミノが完全に消滅したかを返す
    bool has_vanished() const;

    // セルが埋められているかを返す
    bool is_filled(int32 x, int32 y) const;
    
    // セルが埋められているかを返す
    bool is_filled(Point pos) const;
    
    // RingEffectを発生させる
    void add_ring_effect() const;

    // 倒したときにもらえる基礎スコアを計算
    Score get_basic_score() const;

    // 一筆書きのスコアを計算する
    Score get_path_score() const;

    // ランダムで消滅する準備をする
    void prepare_to_randomly_vanish();

    // ポリオミノの左上の座標
    Point upper_left;

   protected:
    // ポリオミノを初期化する
    void initialize(Size grid_size, Cell designated = Cell::None);
    
    // セルの大きさ
    const int cell_size;

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

    // 一筆書き中のパス
    Array<Point> path;

    Array<Point> shuffled_filled_cells;

   private:
    // 一筆書き可能なポリオミノを生成する
    void generate_polyomino(Cell designated);
    
    // セルを生成する
    Cell generate_cell(Cell designated) const;
    
    // ポリオミノをグリッドの左上に寄せてgrid_sizeを詰める
    void shrink_to_fit();

    // ポリオミノが倒されて消滅していく最中のインデックス
    Optional<size_t> vanishing_idx = none;

    bool vanished;

    Effect effect;
    
    static constexpr Color path_color{MyColor::Yellow};
};
