#pragma once
#include "Common.hpp"

// セルの情報をもつクラス
enum class Cell : uint8 {
    None,
    Red,
    Blue,
    Green,
    Poison,
    Alpha,
};

class Polyomino {
   public:
    Polyomino(const Size grid_size, const int32 tolerance);

    // セルが埋められているかを返す
    bool is_filled(int32 x, int32 y) const;

    // 左上の座標を指定してポリオミノを描画する
    void draw(Point upper_left) const;
    
    // bodyからシードを生成する
    int64 generate_seed()const;

   private:
    const Size grid_size;
    
    Grid<Cell> body;
    
    static constexpr Size cell_size{50, 50};

    static constexpr std::array<Point, 4> directions = {
        Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};
};
