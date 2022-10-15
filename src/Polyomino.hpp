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
    Polyomino(Size grid_size, Size cell_size,Point upper_left,int32 tolerance,Cell designated=Cell::None);

    // セルが埋められているかを返す
    bool is_filled(int32 x, int32 y) const;

    // ポリオミノを描画する
    void draw() const;
    
    // bodyからシードを生成する
    int64 generate_seed()const;

protected:
    const Size grid_size;
    
    const Size cell_size;
    
    const Point upper_left;
    
    Grid<Cell> cells;
    
    // セルを表すRect
    Grid<Optional<Rect>> rects;
    
    // パスを描いている途中か
    bool drawing_path=false;
    
    // 一筆書き中のパス
    Array<Rect> path;

private:
    static constexpr std::array<Point, 4> directions = {
        Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};
    
    Cell random_cell(Cell designated)const;
};
