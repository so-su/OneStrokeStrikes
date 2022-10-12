#include "Common.hpp"

// セルの情報をもつクラス
enum class Cell : uint8 {
    None,
    Red,
    Blue,
    Green,
    Poison,
};

class Polyomino {
   public:
    Polyomino();

    // セルが埋められているかを返す
    bool is_filled(int32 x, int32 y) const;

    // 左上の座標を指定してポリオミノを描画する
    void draw(Point upper_left) const;

   private:
    static constexpr Size grid_size{7, 7};

    static constexpr std::array<Point, 4> directions = {
        Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};

    Size cell_size{50, 50};

    Grid<Cell> body{grid_size, Cell::None};
};
