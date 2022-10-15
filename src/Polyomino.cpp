#include "Polyomino.hpp"

Polyomino::Polyomino(const Size grid_size, const int32 tolerance,Cell designated):grid_size(grid_size),body{grid_size, Cell::None} {
    // スタートする点をランダムに決め、セルを埋める
    int32 now_x = Random<int32>(grid_size.x - 1);
    int32 now_y = Random<int32>(grid_size.y - 1);
    body[now_x][now_y] = random_cell(designated);

    int32 error_cnt = 0;
    Cell prev = body[now_x][now_y];

    // ランダムな方向に進みながらセルを埋め、ポリオミノをつくる
    while (error_cnt < tolerance) {
        const auto dir_idx = Random<uint32>(3);
        const auto next_x = now_x + directions[dir_idx].x;
        const auto next_y = now_y + directions[dir_idx].y;

        if (next_x < 0 || grid_size.x <= next_x || next_y < 0 ||
            grid_size.y <= next_y || is_filled(next_x, next_y)) {
            ++error_cnt;
            continue;
        }

        now_x = next_x;
        now_y = next_y;

        // 50%の確率で前の色と同じにする
        if (RandomBool()) {
            body[now_x][now_y] =random_cell(designated);
        } else {
            body[now_x][now_y] = prev;
        }
        prev = body[now_x][now_y];
    }
}

// セルが埋められているかを返す
bool Polyomino::is_filled(int32 x, int32 y) const {
    return body[x][y] != Cell::None;
}

// bodyからシードを生成する
int64 Polyomino::generate_seed()const{
    int64 seed=0;
    for(auto i:step(grid_size.x)){
        for(auto j:step(grid_size.y)){
            if(is_filled(i, j)){
                auto shift=i*grid_size.x+j;
                seed+=((int64)1)<<shift;
            }
        }
    }
    return seed;
}

// 左上の座標を指定してポリオミノを描画する
void Polyomino::draw(Point upper_left,Size cell_size) const {
    for (auto i : step(grid_size.x)) {
        for (auto j : step(grid_size.y)) {
            if (body[i][j] == Cell::None) continue;

            Color cell_color;
            if (body[i][j] == Cell::Red) {
                cell_color = Color{255, 40, 0};
            } else if (body[i][j] == Cell::Blue) {
                cell_color = Color{0, 65, 255};
            } else if (body[i][j] == Cell::Green) {
                cell_color = Color{53, 161, 107};
            }else if(body[i][j]==Cell::Alpha){
                cell_color=Palette::Black;
            }
            Rect{upper_left.x + i * cell_size.x, upper_left.y + j * cell_size.y,
                 cell_size}
                .draw(cell_color)
                .drawFrame(1, 1, Palette::Dimgray);
        }
    }
}

Cell Polyomino::random_cell(Cell designated){
    if(designated!=Cell::None){
        return designated;
    }
    else{
        return Sample({Cell::Red, Cell::Blue, Cell::Green});
    }
}
