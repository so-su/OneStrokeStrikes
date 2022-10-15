#include "Polyomino.hpp"

Polyomino::Polyomino(Size grid_size, Size cell_size,Point upper_left,const int32 tolerance,Cell designated):grid_size(grid_size),cell_size(cell_size),upper_left(upper_left),cells{grid_size, Cell::None},rects{grid_size} {
    // スタートする点をランダムに決め、セルを埋める
    int32 now_x = Random<int32>(grid_size.x - 1);
    int32 now_y = Random<int32>(grid_size.y - 1);
    cells[now_x][now_y] = random_cell(designated);

    int32 error_cnt = 0;
    Cell prev = cells[now_x][now_y];

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
            cells[now_x][now_y] =random_cell(designated);
        } else {
            cells[now_x][now_y] = prev;
        }
        prev = cells[now_x][now_y];
    }
    
    // rectsの初期化
    for(auto i:step(grid_size.x)){
        for(auto j:step(grid_size.y)){
            if(cells[i][j]==Cell::None)continue;
            rects[i][j]=Rect{upper_left.x+i*cell_size.x,upper_left.y+j*cell_size.y,cell_size};
        }
    }
}

// セルが埋められているかを返す
bool Polyomino::is_filled(int32 x, int32 y) const {
    return cells[x][y] != Cell::None;
}

// cellsからシードを生成する
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

// ポリオミノを描画する
void Polyomino::draw() const {
    for (auto i : step(grid_size.x)) {
        for (auto j : step(grid_size.y)) {
            if (cells[i][j] == Cell::None) continue;

            Color cell_color;
            if (cells[i][j] == Cell::Red) {
                cell_color = Color{255, 40, 0};
            } else if (cells[i][j] == Cell::Blue) {
                cell_color = Color{0, 65, 255};
            } else if (cells[i][j] == Cell::Green) {
                cell_color = Color{53, 161, 107};
            }else if(cells[i][j]==Cell::Alpha){
                cell_color=Palette::Black;
            }
            rects[i][j]->draw(cell_color).drawFrame(1, 1, Palette::Dimgray);
        }
    }
}

Cell Polyomino::random_cell(Cell designated)const{
    if(designated!=Cell::None){
        return designated;
    }
    else{
        return Sample({Cell::Red, Cell::Blue, Cell::Green});
    }
}
