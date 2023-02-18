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
