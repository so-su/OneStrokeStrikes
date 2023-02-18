#include "AlphaEnemy.hpp"

AlphaEnemy::AlphaEnemy()
    : Polyomino(Size{8, 20}, Size{30, 30}, Point{700, 150}),
      component_id{8, 20} {
    initialize();
}

// 初期化する
void AlphaEnemy::initialize() {
    generate_polyomino(Size{20, 8}, 100, Cell::Yellow);
    compute_perimeters();
}

// 各連結成分の外周を計算する
void AlphaEnemy::compute_perimeters() {
    perimeters.clear();
    component_id.fill(none);

    num_components = 0;
    Grid<bool> visited(grid_size.y, grid_size.x, false);
    Array<Point> stack;
    Grid<std::array<Optional<Point>, 4>> graph(grid_size.y + 1,
                                               grid_size.x + 1,{none, none, none, none});

    // 深さ優先探索して連結成分ごとに見ていき、それぞれの外周を求める
    for (auto [i, j] : step(grid_size)) {
        if (not is_filled(i, j) or visited[i][j]) {
            continue;
        }

        // ここを抜けるごとに新しい連結成分を処理

        // 連結成分に新しい番号をふる
        component_id[i][j] = num_components;

        visited[i][j] = true;
        stack.emplace_back(i, j);

        while (not stack.empty()) {
            auto [x, y] = stack.back();
            stack.pop_back();

            component_id[x][y] = num_components;

            if (not is_filled(x, y - 1)) {
                graph[x][y][0] = Point{x + 1, y};
            } else if (not visited[x][y - 1]) {
                visited[x][y - 1] = true;
                stack.emplace_back(x, y - 1);
            }
            if (not is_filled(x + 1, y)) {
                graph[x + 1][y][1] = Point{x + 1, y + 1};
            } else if (not visited[x + 1][y]) {
                visited[x + 1][y] = true;
                stack.emplace_back(x + 1, y);
            }
            if (not is_filled(x, y + 1)) {
                graph[x + 1][y + 1][2] = Point{x, y + 1};
            } else if (not visited[x][y + 1]) {
                visited[x][y + 1] = true;
                stack.emplace_back(x, y + 1);
            }
            if (not is_filled(x - 1, y)) {
                graph[x][y + 1][3] = Point{x, y};
            } else if (not visited[x - 1][y]) {
                visited[x - 1][y] = true;
                stack.emplace_back(x - 1, y);
            }
        }

        // 一行目の最左の点をスタート点としてパスをつくる
        Point now = Point{i, j};
        int32 prev = 3;
        perimeters.emplace_back();
        do {
            for (int32 d = 1; d <= 4; ++d) {
                int32 next = (prev + d) % 4;
                if (graph[now.x][now.y][next].has_value()) {
                    perimeters[num_components].emplace_back(Line{
                        upper_left + now * cell_size.x,
                        upper_left + *graph[now.x][now.y][next] * cell_size.x});
                    now = *graph[now.x][now.y][next];
                    prev = next;
                    break;
                }
            }
        } while (now != Point{i, j});

        ++num_components;
    }

    gauge_lens.assign(num_components, 0.0);
}

// 埋まっているセルを、指定した個数だけ削除する
void AlphaEnemy::get_damaged(size_t remove_num) {
    while (remove_num > 0 and num_filled_cells > 0) {
        auto [x, y] = shuffled_filled_cells.back();
        shuffled_filled_cells.pop_back();
        cells[x][y] = Cell::None;
        rects[x][y] = none;
        --num_filled_cells;
        --remove_num;
    }

    compute_perimeters();
}


// 埋まっているセルのうち、指定した図形と重なるものを削除する
void AlphaEnemy::get_damaged(AttackShape* attack_shape) {
    const Point cursor_pos = Cursor::Pos();
    for (auto i : step(3)) {
        for (auto j : step(3)) {
            if (not attack_shape->shape[i][j]) {
                continue;
            }
            const Point pos = cursor_pos + Point{30 * (i - 1), 30 * (j - 1)};
            for (auto [x, y] : step(grid_size)) {
                if (rects[x][y].has_value() and rects[x][y]->contains(pos)) {
                    cells[x][y] = Cell::None;
                    rects[x][y] = none;
                    --num_filled_cells;
                }
            }
        }
    }

    compute_perimeters();

    shuffled_filled_cells.clear();
    for (auto [i, j] : step(grid_size)) {
        if (is_filled(i, j)) {
            shuffled_filled_cells.emplace_back(i, j);
        }
    }
    shuffled_filled_cells.shuffle();
}

// 埋まっているセルがあるかを返す
bool AlphaEnemy::is_alive() const { return num_filled_cells > 0; }

// ゲージを更新し、満タンになったゲージの個数を返す
int32 AlphaEnemy::update_gauges(bool speed_up) {
    int32 full = 0;
    for (auto gauge_idx : step(num_components)) {
        gauge_lens[gauge_idx] += Scene::DeltaTime() * (speed_up ? 10.0 : 1.0);

        if (static_cast<size_t>(gauge_lens[gauge_idx]) ==
            std::size(perimeters[gauge_idx])) {
            gauge_lens[gauge_idx] = 0.0;
            ++full;
        }
    }
    return full;
}

// ゲージをリセットする
void AlphaEnemy::reset_gauge() { gauge_lens.fill(0.0); }

// ゲージを描画する
void AlphaEnemy::draw_gauges() const {
    for (auto gauge_idx : step(num_components)) {
        int32 len_int = static_cast<int32>(gauge_lens[gauge_idx]);

        for (auto i : step(len_int)) {
            perimeters[gauge_idx][i].draw(7, Palette::Black);
        }
        perimeters[gauge_idx][len_int]
            .stretched(0,
                       cell_size.x * ((gauge_lens[gauge_idx] - len_int) - 1.0))
            .draw(7, Palette::Black);

        for (auto i : step(len_int)) {
            perimeters[gauge_idx][i].draw(5, gauge_color);
        }
        perimeters[gauge_idx][len_int]
            .stretched(0,
                       cell_size.x * ((gauge_lens[gauge_idx] - len_int) - 1.0))
            .draw(5, gauge_color);
    }
}

// マウスオーバーで色を変化させる
/*
void AlphaEnemy::mouse_over(Point pos){
    Optional<int32> id=get_component_id(pos);

    if(id.has_value()){
        for(size_t cell_idx=0;cell_idx<size(shuffled_filled_cells);++cell_idx){
            if(auto
[x,y]=shuffled_filled_cells[cell_idx];component_id[x][y]==id){
                cells[x][y]=Cell::Gray;
            }
            else{
                cells[x][y]=Cell::Black;
            }
        }
    }
    else{
        for(size_t cell_idx=0;cell_idx<size(shuffled_filled_cells);++cell_idx){
            auto [x,y]=shuffled_filled_cells[cell_idx];
            cells[x][y]=Cell::Black;
        }
    }
}
 */

/*
// 与えられた座標にあるセルが属する連結成分の番号を返す
Optional<int32> AlphaEnemy::get_component_id(Point pos) const {
    Optional<int32> id = none;

    for (size_t i = 0; i < size(shuffled_filled_cells); ++i) {
        if (auto [x, y] = shuffled_filled_cells[i];
            rects[x][y]->contains(pos)) {
            id = component_id[x][y];
            break;
        }
    }

    return id;
}
*/
