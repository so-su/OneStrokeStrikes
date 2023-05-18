#include "AttackShape.hpp"

AttackShape::AttackShape() : shape{3, 3, false} {
    for (auto y : step(3)) {
        for (auto x : step(3)) {
            cell_pos.emplace_back(x, y);
        }
    }

    initialize();
}

// ランダムに図形を初期化する
void AttackShape::initialize() {
    shape.fill(false);

    // 3〜9個のセルでランダムに図形を作る
    const int32 size{Random(3, 9)};
    cell_pos.shuffle();
    for (auto pos_idx : step(size)) {
        shape[cell_pos[pos_idx]] = true;
    }
}

// 図形を描画する
void AttackShape::draw(Point center, Color color) const {
    for (auto y : step(3)) {
        for (auto x : step(3)) {
            if (shape[y][x]) {
                Rect{Arg::center = center + Point{(x - 1) * cell_size,
                                                  (y - 1) * cell_size},
                     cell_size - 2, cell_size - 2}
                    .draw(color);
            }
        }
    }

    // 図形を囲む枠の描画
    const auto frame_size{cell_size * 3 + 2};
    Rect{Arg::center = center, frame_size, frame_size}.drawFrame(
        0, 2, Palette::White);
}

// 時計回りに回転する
void AttackShape::rotate(){
    const auto tmp{shape};
    for(auto y:step(3)){
        for(auto x:step(3)){
            shape[y][x]=tmp[x][2-y];
        }
    }
}
