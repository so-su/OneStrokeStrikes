#include "Button.hpp"

Button::Button(const Array<Point>& path_, Size grid_size_, int32 cell_size_,
               Point upper_left, Color color_, double alpha_min_)
    :grid_size(grid_size_),
     cell_size(cell_size_),
     color(color_),
     path(path_),
     rects(grid_size_, none),
     alpha_transitions(std::size(path), Transition{0.1s, 0.05s}),
     alpha_min{alpha_min_}{
    for (const auto& cell_pos : path) {
        rects[cell_pos] =
            Rect{upper_left + cell_size * cell_pos, cell_size, cell_size};
    }

    /* グリッドの格子点を頂点として有向グラフを構築し、外周を計算する */
    // 隣接リスト
    Grid<std::array<Optional<Point>, 4>> graph(grid_size+Size{1,1});
    for (auto& ary : graph) {
        ary.fill(none);
    }

    // 有向辺をはる
    for (auto y : step(grid_size.y)){
        for(auto x: step(grid_size.x)){
            if (not is_filled(x, y)) continue;
            if (not is_filled(x, y - 1)) {
                graph[y][x][0] = Point{x + 1, y};
            }
            if (not is_filled(x + 1, y)) {
                graph[y][x+1][1] = Point{x + 1, y + 1};
            }
            if (not is_filled(x, y + 1)) {
                graph[y + 1][x + 1][2] = Point{x, y + 1};
            }
            if (not is_filled(x - 1, y)) {
                graph[y+1][x][3] = Point{x, y};
            }
        }
    }

    // 埋まっているセルのうち、1行目の最左のものの左上を始点とする
    Point start;
    for (auto x : step(grid_size.x)) {
        if (is_filled(x, 0)) {
            start = Point{x, 0};
            break;
        }
    }
          
    // 4方向のうち、直前に通ってきた方向をもっておく
    int32 prev{3};
    
    // 外周のパスをつくる
    Point now{start};
    do {
        // 4方向を順番に見ていき辺がはられていたら先に進むのを、一周するまで繰り返す
        for (int32 d = 1; d <= 4; ++d) {
            const int32 next{(prev + d) % 4};
            if (graph[now][next].has_value()) {
                perimeter.emplace_back(
                    upper_left + now * cell_size,
                    upper_left + *graph[now][next] * cell_size);
                now = *graph[now][next];
                prev = next;
                break;
            }
        }
    } while (now != start);
}

// ボタンの描画
void Button::draw() const {
    for (auto cell_idx : step(std::size(path))) {
        rects[path[cell_idx]]->draw(
            ColorF{color, alpha_min + (1.0 - alpha_min) * alpha_transitions[cell_idx].value()});
    }
}

// ゲージの描画
void Button::draw_gauge() const {
    const double gauge_len{
        std::size(perimeter) * (mouseover_transition.value() + 1e-10)};
    const size_t len_int {static_cast<size_t>(gauge_len)};

    // ゲージを黒く縁取り
    for (auto i : step(len_int)) {
        perimeter[i].draw(12, Palette::Black);
    }
    if (len_int < std::size(perimeter)) {
        perimeter[len_int]
            .stretched(0, cell_size * ((gauge_len - len_int) - 1.0))
            .draw(12, Palette::Black);
    }

    // オレンジ色のゲージを描画
    for (auto i : step(len_int)) {
        perimeter[i].draw(10, MyColor::Orange);
    }
    if (len_int < std::size(perimeter)) {
        perimeter[len_int]
            .stretched(0, cell_size * ((gauge_len - len_int) - 1.0))
            .draw(10, MyColor::Orange);
    }
}

// ボタンの状態を更新
void Button::update() {
    const bool contains_cursor{contains(Cursor::Pos())};

    // ボタンが押されたとき
    if (not pressed and MouseL.down() and contains_cursor) {
        pressed = true;
        for (auto cell_idx : step(std::size(path))) {
            if (rects[path[cell_idx]]->contains(Cursor::Pos())) {
                start_index = cell_idx;
                break;
            }
        }
    }

    mouseover_transition.update(contains_cursor);
    pressed_transition.update(pressed);

    const double path_len{static_cast<double>(std::size(path))};

    for (auto dist : step(std::size(path))) {
        const auto cell_idx{(start_index + dist) % std::size(path)};
        alpha_transitions[cell_idx].update(pressed_transition.value() >=
                                           (dist + 1) / path_len);
    }
}

// 入力された座標がボタンと重なっているかを返す
bool Button::contains(Point pos) const {
    for (auto rect : rects) {
        if (rect.has_value() and rect->contains(pos)) {
            return true;
        }
    }
    return false;
}

// ゲージが満タンになっているかを返す
bool Button::completed() const {
    const auto path_len{std::size(path)};
    return alpha_transitions[(start_index + path_len - 1) % path_len].isOne();
}

// ゲージのリセット
void Button::reset(){
    for(auto& transition:alpha_transitions){
        transition = Transition{0.1s, 0.05s};
    }

    pressed = false;
}

// セルが埋まっているかを返す
bool Button::is_filled(int32 x, int32 y) const {
    if (x < 0 or grid_size.x <= x or y < 0 or grid_size.y <= y) {
        return false;
    }
    return rects[y][x].has_value();
}
