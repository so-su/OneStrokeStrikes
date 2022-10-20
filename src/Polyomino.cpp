#include "Polyomino.hpp"

Polyomino::Polyomino(Size max_grid_size,Size cell_size, Point center)
    : cell_size(cell_size),
      center(center),
      cells{max_grid_size, Cell::None},
      rects{max_grid_size,none} {
}

// セルが埋められているかを返す
bool Polyomino::is_filled(int32 x, int32 y) const {
    return cells[x][y] != Cell::None;
}

// cellsからシードを生成する
int64 Polyomino::generate_seed() const {
    int64 seed = 0;
    for (auto i : step(grid_size.x)) {
        for (auto j : step(grid_size.y)) {
            if (is_filled(i, j)) {
                auto shift = i * grid_size.x + j;
                seed += ((int64)1) << shift;
            }
        }
    }
    return seed;
}

// ポリオミノを描画する
void Polyomino::draw() const {
    for (auto [i, j] : step(grid_size)) {
        if (cells[i][j] == Cell::None) continue;

        Color cell_color;
        if (cells[i][j] == Cell::Red) {
            cell_color = Color{255, 40, 0};
        } else if (cells[i][j] == Cell::Blue) {
            cell_color = Color{0, 65, 255};
        } else if (cells[i][j] == Cell::Green) {
            cell_color = Color{53, 161, 107};
        } else if (cells[i][j] == Cell::Black) {
            cell_color = Palette::Black;
        }
        rects[i][j]->draw(cell_color).drawFrame(1, 1, Palette::Dimgray);
    }
}

// パスを描画する
void Polyomino::draw_path() const {
    for (size_t path_idx = 0; path_idx + 1 < size(path); ++path_idx) {
        Vec2 from = rects[path[path_idx].x][path[path_idx].y]->center();
        Vec2 to = rects[path[path_idx + 1].x][path[path_idx + 1].y]->center();
        Line{from, to}.draw(3, Palette::Yellow);
    }
}

// カーソルの座標を受け取ってパスを更新
bool Polyomino::update_path(Point pos) {
    for (auto [i, j] : step(grid_size)) {
        if (rects[i][j] and rects[i][j]->contains(pos)) {
            if (not path.empty()) {
                auto [last_i, last_j] = path.back();
                // パスの末尾のセルに隣接していないなら拒否
                if (not(abs(i - last_i) + abs(j - last_j) == 1)) {
                    return false;
                }

                // パスに既に含まれていたら拒否
                for (size_t path_idx = 0; path_idx < size(path); ++path_idx) {
                    if (path[path_idx] == Point{i, j}) {
                        // 末尾から2番目ならパスの末尾を削除
                        if (path_idx + 2 == size(path)) {
                            path.pop_back();
                            return true;
                        }
                        return false;
                    }
                }
            }
            path.emplace_back(i, j);
            // パスがポリオミノを覆ったならポリオミノの消滅が始まる
            if (size(path) == cell_num) {
                vanishing_idx = 0;
            }
            return true;
        }
    }

    // posがポリオミノ外のとき
    return false;
}

// パスを削除
void Polyomino::clear_path() { path.clear(); }

// パスを逆順にする
void Polyomino::reverse_path(){
    std::reverse(std::begin(path),std::end(path));
}

// ポリオミノの消滅を進める、すべて消滅したらtrueを返す
bool Polyomino::vanish() {
    if (vanishing_idx == cell_num) {
        return true;
    }
    cells[path[*vanishing_idx].x][path[*vanishing_idx].y] = Cell::None;
    ++(*vanishing_idx);
    return false;
}

// ポリオミノが消滅中かを返す
bool Polyomino::is_vanishing() const { return vanishing_idx.has_value(); }

// ポリオミノの初期化
void Polyomino::initialize(Size grid_size_,const int32 tolerance, Cell designated){
    grid_size=grid_size_;
    cells.fill(Cell::None);
    rects.fill(none);
    cell_num=1;
    clear_path();
    vanishing_idx=none;
    
    // スタートする点をランダムに決め、セルを埋める
    int32 start_x=Random<int32>(grid_size.x - 1);
    int32 start_y=Random<int32>(grid_size.y - 1);
    cells[start_x][start_y] = generate_cell(designated);
    
    for(auto _ :step(2)){
        int32 now_x = start_x;
        int32 now_y = start_y;

        int32 error_cnt = 0;
        Cell prev = cells[start_x][start_y];

        // ランダムな方向に進みながらセルを埋め、ポリオミノをつくる
        while (error_cnt < tolerance) {
            const auto dir_idx = Random<uint32>(3);
            const auto next_x = now_x + directions[dir_idx].x;
            const auto next_y = now_y + directions[dir_idx].y;

            if (next_x < 0 or grid_size.x <= next_x or next_y < 0 or
                grid_size.y <= next_y or is_filled(next_x, next_y)) {
                ++error_cnt;
                continue;
            }

            now_x = next_x;
            now_y = next_y;

            // 50%の確率で前の色と同じにする
            if (RandomBool()) {
                cells[now_x][now_y] = generate_cell(designated);
            } else {
                cells[now_x][now_y] = prev;
            }
            ++cell_num;

            prev = cells[now_x][now_y];
        }
    }
    
    // 余白を詰める
    resize();
    
    // ポリオミノの左上の座標を求める
    upper_left=center-cell_size.x*grid_size/2;

    // rectsの初期化
    for (auto [i, j] : step(grid_size)) {
        if (cells[i][j] == Cell::None) continue;
        rects[i][j] = Rect{upper_left.x + i * cell_size.x,
                           upper_left.y + j * cell_size.y, cell_size};
    }
}

void Polyomino::resize(){
    int32 exist_min_x=grid_size.x;
    int32 exist_max_x=-1;
    int32 exist_min_y=grid_size.y;
    int32 exist_max_y=-1;
    for(auto [x,y]:step(grid_size)){
        if(cells[x][y]==Cell::None)continue;
        exist_min_x=Min(x,exist_min_x);
        exist_max_x=Max(x,exist_max_x);
        exist_min_y=Min(y,exist_min_y);
        exist_max_y=Max(y,exist_max_y);
    }
    auto tmp=cells;
    cells.fill(Cell::None);
    
    grid_size=Size{exist_max_x+1-exist_min_x,exist_max_y+1-exist_min_y};
    for(auto [i,j]:step(grid_size)){
        cells[i][j]=tmp[exist_min_x+i][exist_min_y+j];
    }
}

// セルを生成する
Cell Polyomino::generate_cell(Cell designated) const {
    if (designated != Cell::None) {
        return designated;
    } else {
        return Sample({Cell::Red, Cell::Blue, Cell::Green});
    }
}
