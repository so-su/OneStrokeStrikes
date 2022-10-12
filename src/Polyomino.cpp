#include "Polyomino.hpp"

Polyomino::Polyomino() {
    for (auto i : step(grid_size.x)) {
        for (auto j : step(grid_size.y)) {
            body[i][j] =
                Sample({Cell::None, Cell::Red, Cell::Blue, Cell::Green});
        }
    }
}

// 左上の座標を受け取って描画
void Polyomino::draw(Point upper_left) const {
    for (auto i : step(grid_size.x)) {
        for (auto j : step(grid_size.y)) {
            if (body[i][j] == Cell::None) continue;
            Color cell_color;
            if (body[i][j] == Cell::Red) {
                cell_color = Palette::Red;
            } else if (body[i][j] == Cell::Blue) {
                cell_color = Palette::Blue;
            } else if (body[i][j] == Cell::Green) {
                cell_color = Palette::Green;
            }
            Rect{upper_left.x + i * cell_size.x, upper_left.y + j * cell_size.y,
                 cell_size}
                .draw(cell_color);
        }
    }
}
