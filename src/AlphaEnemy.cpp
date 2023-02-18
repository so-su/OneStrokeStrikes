#include "AlphaEnemy.hpp"

AlphaEnemy::AlphaEnemy()
    : Polyomino(Size{20, 8}, 30, Point{700, 150}),
      component_id{20, 8, none} {
    initialize();
}

// 初期化する
void AlphaEnemy::initialize() {
    Polyomino::initialize(Size{20, 8}, Cell::Yellow);
    compute_perimeters();
}

// 各連結成分の外周を計算する
void AlphaEnemy::compute_perimeters() {
    perimeters.clear();
    component_id.fill(none);
    num_components = 0;
    
    // 訪問済みのセルかどうか
    Grid<bool> visited(grid_size, false);
    
    // 訪問中のセルを入れておく配列
    Array<Point> stack;
    
    // 各格子点から出ていく辺を4方向について管理する
    Grid<std::array<Optional<Point>, 4>> graph{grid_size + Size{1, 1} ,{none, none, none, none}};

    // 深さ優先探索しながら連結成分ごとに見ていき、それぞれの外周を求める
    for (auto component_upper_left : step(grid_size)) {
        if (not is_filled(component_upper_left) or visited[component_upper_left]) {
            continue;
        }

        /* ここを抜けるごとに新しい連結成分を処理 */

        // 連結成分に新しい番号をふる
        component_id[component_upper_left] = num_components;

        visited[component_upper_left] = true;
        stack.emplace_back(component_upper_left);

        while (not stack.empty()) {
            const auto [x, y] = stack.back();
            stack.pop_back();

            component_id[y][x] = num_components;

            // 上のセルが埋まっていないなら右向きに辺をはる
            if (not is_filled(x, y - 1)) {
                graph[y][x][0] = Point{x + 1, y};
            } else if (not visited[y - 1][x]) {
                visited[y - 1][x] = true;
                stack.emplace_back(x, y - 1);
            }
            
            // 右のセルが埋まっていないなら下向きに辺をはる
            if (not is_filled(x + 1, y)) {
                graph[y][x + 1][1] = Point{x + 1, y + 1};
            } else if (not visited[y][x + 1]) {
                visited[y][x + 1] = true;
                stack.emplace_back(x + 1, y);
            }
            
            // 下のセルが埋まっていないなら左向きに辺をはる
            if (not is_filled(x, y + 1)) {
                graph[y + 1][x + 1][2] = Point{x, y + 1};
            } else if (not visited[y + 1][x]) {
                visited[y + 1][x] = true;
                stack.emplace_back(x, y + 1);
            }
            
            // 左のセルが埋まっていないなら上向きに辺をはる
            if (not is_filled(x - 1, y)) {
                graph[y + 1][x][3] = Point{x, y};
            } else if (not visited[y][x - 1]) {
                visited[y][x - 1] = true;
                stack.emplace_back(x - 1, y);
            }
        }

        // はじめに訪問したセルの左上を始点とする
        Point now{component_upper_left};
        
        // 4方向のうち、直前に通ってきた方向をもっておく
        int32 prev_dir{3};
        
        perimeters.emplace_back();
        
        // ポリオミノの外周のパスをつくる
        do {
            // 4方向を順番に見ていき辺がはられていたら先に進むのを、一周するまで繰り返す
            for (auto d : Range(1, 5)) {
                const auto next_dir{(prev_dir + d) % 4};
                if (graph[now][next_dir].has_value()) {
                    perimeters[num_components].emplace_back(
                        upper_left + now * cell_size,
                        upper_left + graph[now][next_dir].value() * cell_size);
                    now = graph[now][next_dir].value();
                    prev_dir = next_dir;
                    break;
                }
            }
        } while (now != component_upper_left);

        ++num_components;
    }

    gauge_lens.assign(num_components, 0.0);
}

// 埋まっているセルを、指定した個数だけ削除する
void AlphaEnemy::get_damaged(size_t remove_num) {
    while (remove_num > 0 and num_filled_cells > 0) {
        const auto pos{shuffled_filled_cells.back()};
        shuffled_filled_cells.pop_back();
        cells[pos] = Cell::None;
        rects[pos] = none;
        --num_filled_cells;
        --remove_num;
    }

    compute_perimeters();
}


// 埋まっているセルのうち、指定した図形と重なるものを削除する
void AlphaEnemy::get_damaged(AttackShape* attack_shape) {
    const Point cursor_pos{Cursor::Pos()};
    
    // AttackShapeの埋まっている各セルの中心について、
    // それを含む、ポリオミノの埋まっているセルが存在するか判定する
    for (auto shape_y : step(3)) {
        for (auto shape_x : step(3)) {
            if (not attack_shape->shape[shape_y][shape_x]) {
                continue;
            }
            const Point center_pos{cursor_pos + Point{30 * (shape_x - 1), 30 * (shape_y - 1)}};
            for (auto pos : step(grid_size)) {
                if (rects[pos].has_value() and rects[pos]->contains(center_pos)) {
                    cells[pos] = Cell::None;
                    rects[pos] = none;
                    --num_filled_cells;
                }
            }
        }
    }

    compute_perimeters();

    shuffled_filled_cells.clear();
    for (auto pos : step(grid_size)) {
        if (is_filled(pos)) {
            shuffled_filled_cells.emplace_back(pos);
        }
    }
    shuffled_filled_cells.shuffle();
}

// 埋まっているセルがあるかを返す
bool AlphaEnemy::is_alive() const { return num_filled_cells > 0; }

// ゲージを更新し、満タンになったゲージの個数を返す
int32 AlphaEnemy::update_gauges() {
    int32 full{0};
    for (auto gauge_idx : step(num_components)) {
        gauge_lens[gauge_idx] += Scene::DeltaTime();

        // ゲージが満タンになったとき
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

        // ゲージを縁取りする
        for (auto line_idx : step(len_int)) {
            perimeters[gauge_idx][line_idx].draw(7, Palette::Black);
        }
        perimeters[gauge_idx][len_int]
            .stretched(0,
                       cell_size * ((gauge_lens[gauge_idx] - len_int) - 1.0))
            .draw(7, Palette::Black);

        // ゲージの本体を描画する
        for (auto line_idx : step(len_int)) {
            perimeters[gauge_idx][line_idx].draw(5, gauge_color);
        }
        perimeters[gauge_idx][len_int]
            .stretched(0,
                       cell_size * ((gauge_lens[gauge_idx] - len_int) - 1.0))
            .draw(5, gauge_color);
    }
}

/*
// マウスオーバーで色を変化させる
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
