#include "Common.hpp"

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

    // 左上の座標を受け取って描画
    void draw(Point upper_left) const;

   private:
    static constexpr Size grid_size{5, 5};

    Size cell_size{50, 50};

    Grid<Cell> body{grid_size, Cell::None};
};
