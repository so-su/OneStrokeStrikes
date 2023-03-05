#include "Enemy.hpp"

Enemy::Enemy(Point center)
    : Polyomino(Size{8, 8}, 50, center){
    initialize();
}

// 初期化する
void Enemy::initialize() {
    const int32 grid_len{Random(3, 8)};
    Polyomino::initialize(Size{grid_len, grid_len});
    compute_perimeter();
    gauge_speed = 1.0;
    clear_path();
}

// 外周を計算する
void Enemy::compute_perimeter() {
    gauge_len = 0.0;
    perimeter.clear();

    // 各格子点から出ていく辺を4方向について管理する
    Grid<std::array<Optional<Point>, 4>> graph{grid_size + Size{1,1}, {none,none,none,none}};

    // ポリオミノの各セルを見ていき、グラフを構成する
    for (auto [x, y] : step(grid_size)) {
        if (not is_filled(x, y)) continue;
        
        // 上のセルが埋まっていないなら右向きに辺をはる
        if (not is_filled(x, y - 1)) {
            graph[y][x][0] = Point{x + 1, y};
        }
        
        // 右のセルが埋まっていないなら下向きに辺をはる
        if (not is_filled(x + 1, y)) {
            graph[y][x+1][1] = Point{x + 1, y + 1};
        }
        
        // 下のセルが埋まっていないなら左向きに辺をはる
        if (not is_filled(x, y + 1)) {
            graph[y + 1][x + 1][2] = Point{x, y + 1};
        }
        
        // 左のセルが埋まっていないなら上向きに辺をはる
        if (not is_filled(x - 1, y)) {
            graph[y + 1][x][3] = Point{x, y};
        }
    }

    // 埋まっているセルのうち、y=0でxが最小のセルの左上を始点とする
    Point start;
    for (auto x : step(grid_size.x)) {
        if (is_filled(x, 0)) {
            start = Point{x, 0};
            break;
        }
    }
    Point now{start};
    
    // 4方向のうち、直前に通ってきた方向をもっておく
    int32 prev_dir{3};
    
    // ポリオミノの外周のパスをつくる
    do {
        // 4方向を順番に見ていき辺がはられていたら先に進むのを、一周するまで繰り返す
        for (auto d : Range(1,5)) {
            const auto next_dir{(prev_dir + d) % 4};
            if (graph[now][next_dir].has_value()) {
                perimeter.emplace_back(
                    upper_left + now * cell_size,
                    upper_left + *graph[now][next_dir] * cell_size);
                now = *graph[now][next_dir];
                prev_dir = next_dir;
                break;
            }
        }
    } while (now != start);
}

// パスを描画する
void Enemy::draw_path() const {
    if (path.empty()) {
        return;
    }

    // 線を描く
    for (size_t path_idx = 0; path_idx + 1 < std::size(path); ++path_idx) {
        const Vec2 from{rects[path[path_idx]]->center()};
        const Vec2 to{rects[path[path_idx + 1]]->center()};
        Line{from, to}.draw(3, path_color);
    }

    // 始点に円を描く
    Circle{rects[path.front()]->center(), 5}.draw(path_color);

    // 終点に三角形を描く
    if (std::size(path) >= 2) {
        const Vec2 from {rects[*next(std::rbegin(path))]->center()};
        const Vec2 to{rects[path.back()]->center()};
        
        // 最後の2点の座標を見て三角形の方向を決める
        // 誤差も考慮して判定する
        if (to.x < from.x - 0.5) {
            Triangle{to, 20, 270_deg}.draw(path_color);
        } else if (to.x > from.x + 0.5) {
            Triangle{to, 20, 90_deg}.draw(path_color);
        } else if (to.y < from.y - 0.5) {
            Triangle{to, 20, 0_deg}.draw(path_color);
        } else if (to.y > from.y + 0.5) {
            Triangle{to, 20, 180_deg}.draw(path_color);
        }
    }
}

// カーソルの座標を受け取ってパスを更新
void Enemy::update_path(Point cursor_pos) {
    for (auto pos : step(grid_size)) {
        if (rects[pos] and rects[pos]->contains(cursor_pos)) {
            if (not path.empty()) {
                // パスの末尾のセルに隣接していないなら拒否
                if (not(std::abs(pos.x - path.back().x) + std::abs(pos.y - path.back().y) == 1)) {
                    return;
                }

                // パスに既に含まれていたら拒否
                // ただし、末尾から2番目ならパスの末尾を削除
                for (auto path_idx : step(std::size(path))) {
                    if (path[path_idx] == pos) {
                        if (path_idx + 2 == std::size(path)) {
                            path.pop_back();
                        }
                        return;
                    }
                }
            }
            
            /* ここを抜けたときにパスが伸びる */
            
            path.emplace_back(pos);
            
            // パスがポリオミノを覆ったならポリオミノの消滅が始まる
            if (std::size(path) == num_filled_cells) {
                // パスの後ろから削除予定スタックに入れていく
                for(auto it = path.rbegin(); it != path.rend(); ++it){
                    cells_to_erase.emplace_back(*it);
                }
                
                add_ring_effect();
            }
            
            return;
        }
    }
    
    /* cursor_posがポリオミノ外にあるとき、ここに到達する */
}

// パスを削除する
void Enemy::clear_path() { path.clear(); }

// パスを反転させる
void Enemy::reverse_path() {
    std::reverse(std::begin(path), std::end(path));
}

// ゲージを更新し、満タンになったらtrueを返す
bool Enemy::update_gauge() {
    gauge_len += Scene::DeltaTime() * gauge_speed;

    // ゲージが満タンになったとき
    if (static_cast<size_t>(gauge_len) == std::size(perimeter)) {
        gauge_len = 0.0;
        return true;
    }
    
    return false;
}

// ゲージをリセットする
void Enemy::reset_gauge() { gauge_len = 0.0; }

// 外周のゲージを描画する
void Enemy::draw_gauge() const {
    const int32 len_int{static_cast<int32>(gauge_len)};

    // ゲージを縁取りする
    for (auto line_idx : step(len_int)) {
        perimeter[line_idx].draw(7, ColorF{0.25});
    }
    perimeter[len_int]
        .stretched(0, cell_size * ((gauge_len - len_int) - 1.0))
        .draw(7, ColorF{0.25});

    // ゲージの本体を描画する
    for (auto line_idx : step(len_int)) {
        perimeter[line_idx].draw(5, gauge_color);
    }
    perimeter[len_int]
        .stretched(0, cell_size * ((gauge_len - len_int) - 1.0))
        .draw(5, gauge_color);
}

// 攻撃力を返す
int32 Enemy::attack_value() const { return 100; }

// ゲージの周回速度を上昇させる
void Enemy::speed_up_gauge(int32 times) { gauge_speed += 0.3 * times; }

// ランダムで消滅する準備をする
void Enemy::prepare_to_randomly_vanish() {
    cells_to_erase = shuffled_filled_cells;
}

// 倒したときにもらえる基礎スコアを計算
Score Enemy::get_basic_score() const {
    Score score{0, 0, 0, 0, 0, 0};
    for (auto pos : step(grid_size)) {
        if (cells[pos] == Cell::Green) {
            score.green += 10;
        } else if (cells[pos] == Cell::Red) {
            score.red += 10;
        } else if (cells[pos] == Cell::Blue) {
            score.blue += 10;
        }
    }
    return score;
}

// 一筆書きのスコアを計算する
Score Enemy::get_path_score() const {
    Score score{get_basic_score()};
    
    Cell now_color{cells[path.front()]};
    int32 len{1};
    
    // ボーナスを加算するためのラムダ式
    auto add_bonus=[&score, &now_color, &len]()->void{
        const auto bonus{len * len}; // 長さの2乗
        if (now_color == Cell::Green) {
            score.green += bonus;
        } else if (now_color == Cell::Red) {
            score.red += bonus;
        } else if (now_color == Cell::Blue) {
            score.blue += bonus;
        }
    };
    
    // パスをセルの色でランレングス圧縮し、それぞれに対して長さによるボーナスを加算する
    for (size_t path_idx = 1; path_idx < std::size(path); ++path_idx) {
        if (cells[path[path_idx]] == now_color) {
            ++len;
        } else {
            add_bonus();
            now_color = cells[path[path_idx]];
            len = 1;
        }
    }
    // 最後の連続部分のボーナスも忘れずに加算する
    add_bonus();
    
    // 端点の色を計上するためのラムダ式
    auto add_endpoint=[&score](Cell cell)->void{
        if (cell == Cell::Green) {
            ++score.green_endpoint;
        } else if (cell == Cell::Red) {
            ++score.red_endpoint;
        } else if (cell == Cell::Blue) {
            ++score.blue_endpoint;
        }
    };

    // 始点と終点の色を計上する
    add_endpoint(cells[path.front()]);
    add_endpoint(cells[path.back()]);

    return score;
}
