#include "Enemy.hpp"

Enemy::Enemy(Point center)
    : Polyomino(Size{8, 8}, Size{50, 50}, center), gauge_len(0.0) {
    initialize();
}

void Enemy::initialize() {
    int32 grid_len = Random(3, 8);
    Polyomino::initialize(Size{grid_len, grid_len}, grid_len * grid_len);

    gauge_len = 0.0;
    gauge_speed = 1.0;
    perimeter.clear();

    // 隣接する点の座標
    Grid<std::array<Optional<Point>, 4>> graph(grid_size.y + 1,
                                               grid_size.x + 1);
    for (auto &ary : graph) {
        ary.fill(none);
    }

    // ポリオミノの外周のパスを計算する
    for (auto [x, y] : step(grid_size)) {
        if (not is_filled(x, y)) continue;
        if (not is_filled(x, y - 1)) {
            graph[x][y][0] = Point{x + 1, y};
        }
        if (not is_filled(x + 1, y)) {
            graph[x + 1][y][1] = Point{x + 1, y + 1};
        }
        if (not is_filled(x, y + 1)) {
            graph[x + 1][y + 1][2] = Point{x, y + 1};
        }
        if (not is_filled(x - 1, y)) {
            graph[x][y + 1][3] = Point{x, y};
        }
    }

    // 一行目の最左の点をスタート点としてパスをつくる
    Point start;
    for (auto x : step(grid_size.x)) {
        if (is_filled(x, 0)) {
            start = Point{x, 0};
            break;
        }
    }
    Point now = start;
    int32 prev = 3;
    do {
        for (int32 d = 1; d <= 4; ++d) {
            int32 next = (prev + d) % 4;
            if (graph[now.x][now.y][next].has_value()) {
                perimeter.emplace_back(Line{
                    upper_left + now * cell_size.x,
                    upper_left + *graph[now.x][now.y][next] * cell_size.x});
                now = *graph[now.x][now.y][next];
                prev = next;
                break;
            }
        }
    } while (now != start);
}

// ゲージを更新し、満タンになったらtrueを返す
bool Enemy::update_gauge() {
    gauge_len += Scene::DeltaTime() * gauge_speed;

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
    int32 len_int = static_cast<int32>(gauge_len);

    for (auto i : step(len_int)) {
        perimeter[i].draw(7, ColorF{0.25});
    }
    perimeter[len_int]
        .stretched(0, cell_size.x * ((gauge_len - len_int) - 1.0))
        .draw(7, ColorF{0.25});

    for (auto i : step(len_int)) {
        perimeter[i].draw(5, gauge_color);
    }
    perimeter[len_int]
        .stretched(0, cell_size.x * ((gauge_len - len_int) - 1.0))
        .draw(5, gauge_color);
}

int32 Enemy::attack_value() const { return 100; }

void Enemy::speed_up_gauge(int32 times) { gauge_speed += 0.3 * times; }
