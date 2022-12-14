#include "AlphaEnemy.hpp"

AlphaEnemy::AlphaEnemy()
    : Polyomino(Size{8, 20}, Size{30, 30}, Point{700, 150}),
      component_id{8, 20} {
    initialize();
}

void AlphaEnemy::initialize() {
    Polyomino::initialize(Size{20, 8}, 100, Cell::Yellow);
    new_shape_initialize();
}

void AlphaEnemy::new_shape_initialize() {
    perimeters.clear();
    component_id.fill(none);

    // 幅優先探索して連結成分ごとに見ていき、それぞれの外周を求める
    num_component = 0;
    Grid<bool> seen(grid_size.y, grid_size.x, false);
    Array<Point> tmp;
    Grid<std::array<Optional<Point>, 4>> graph(grid_size.y + 1,
                                               grid_size.x + 1);
    for (auto& ary : graph) {
        ary.fill(none);
    }

    for (auto [i, j] : step(grid_size)) {
        if (not is_filled(i, j) or seen[i][j]) {
            continue;
        }

        // ここを抜けるごとに新しい連結成分を処理

        // 連結成分に新しい番号をふる
        component_id[i][j] = num_component;

        seen[i][j] = true;
        tmp.emplace_back(i, j);

        while (not tmp.empty()) {
            auto [x, y] = tmp.back();
            tmp.pop_back();

            component_id[x][y] = num_component;

            if (not is_filled(x, y - 1)) {
                graph[x][y][0] = Point{x + 1, y};
            } else if (not seen[x][y - 1]) {
                seen[x][y - 1] = true;
                tmp.emplace_back(x, y - 1);
            }
            if (not is_filled(x + 1, y)) {
                graph[x + 1][y][1] = Point{x + 1, y + 1};
            } else if (not seen[x + 1][y]) {
                seen[x + 1][y] = true;
                tmp.emplace_back(x + 1, y);
            }
            if (not is_filled(x, y + 1)) {
                graph[x + 1][y + 1][2] = Point{x, y + 1};
            } else if (not seen[x][y + 1]) {
                seen[x][y + 1] = true;
                tmp.emplace_back(x, y + 1);
            }
            if (not is_filled(x - 1, y)) {
                graph[x][y + 1][3] = Point{x, y};
            } else if (not seen[x - 1][y]) {
                seen[x - 1][y] = true;
                tmp.emplace_back(x - 1, y);
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
                    perimeters[num_component].emplace_back(Line{
                        upper_left + now * cell_size.x,
                        upper_left + *graph[now.x][now.y][next] * cell_size.x});
                    now = *graph[now.x][now.y][next];
                    prev = next;
                    break;
                }
            }
        } while (now != Point{i, j});

        ++num_component;
    }

    gauge_lens.assign(num_component, 0.0);
}

void AlphaEnemy::get_damaged(size_t remove_num) {
    // 埋まっているセルの中から remove_num 個選んで削除する
    while (remove_num > 0 and cell_num > 0) {
        auto [x, y] = shuffled_filled_cells.back();
        shuffled_filled_cells.pop_back();
        cells[x][y] = Cell::None;
        rects[x][y] = none;
        --cell_num;
        --remove_num;
    }

    new_shape_initialize();
}

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
                    --cell_num;
                }
            }
        }
    }

    new_shape_initialize();

    shuffled_filled_cells.clear();
    for (auto [i, j] : step(grid_size)) {
        if (is_filled(i, j)) {
            shuffled_filled_cells.emplace_back(i, j);
        }
    }
    shuffled_filled_cells.shuffle();
}

bool AlphaEnemy::is_alive() const { return cell_num > 0; }

// ゲージを更新し、満タンになったゲージの個数を返す
int32 AlphaEnemy::update_gauges(bool speed_up) {
    int32 full = 0;
    for (auto gauge_idx : step(num_component)) {
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

// 外周のゲージを描画する
void AlphaEnemy::draw_gauges() const {
    for (auto gauge_idx : step(num_component)) {
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

// 座標 pos にあるセルが属する連結成分の番号を返す
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
