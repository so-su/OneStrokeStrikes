#include "Button.hpp"

Button::Button(const Array<Point>& path_, Size grid_size_, int32 cell_size_,
               Point upper_left, Color color_)
    : path(path_),
      rects(grid_size_.y, grid_size_.x, none),
      grid_size(grid_size_),
      cell_size(cell_size_),
      color(color_),
      mouseover_transition{0.2s, 0.1s},
      pressed_transition{0.8s, 0.4s},
      alpha_transitions(std::size(path), Transition{0.1s, 0.05s}) {
    for (const auto& cell_pos : path) {
        rects[cell_pos.x][cell_pos.y] =
            Rect{upper_left + cell_size * cell_pos, cell_size, cell_size};
    }

    // 隣接する点の座標
    Grid<std::array<Optional<Point>, 4>> graph(grid_size.y + 1,
                                               grid_size.x + 1);
    for (auto& ary : graph) {
        ary.fill(none);
    }

    // 外周のパスを計算する
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
                perimeter.emplace_back(
                    upper_left + now * cell_size,
                    upper_left + *graph[now.x][now.y][next] * cell_size);
                now = *graph[now.x][now.y][next];
                prev = next;
                break;
            }
        }
    } while (now != start);
}

void Button::draw() const {
    for (auto cell_idx : step(std::size(path))) {
        auto [x, y] = path[cell_idx];
        rects[x][y]->draw(
            ColorF{color, 0.7 + 0.3 * alpha_transitions[cell_idx].value()});
    }
}

void Button::draw_gauge() const {
    const double gauge_len =
        std::size(perimeter) * (mouseover_transition.value() + 1e-10);
    const size_t len_int = static_cast<size_t>(gauge_len);

    for (auto i : step(len_int)) {
        perimeter[i].draw(12, Palette::Black);
    }
    if (len_int < std::size(perimeter)) {
        perimeter[len_int]
            .stretched(0, cell_size * ((gauge_len - len_int) - 1.0))
            .draw(12, Palette::Black);
    }

    for (auto i : step(len_int)) {
        perimeter[i].draw(10, MyColor::Orange);
    }
    if (len_int < std::size(perimeter)) {
        perimeter[len_int]
            .stretched(0, cell_size * ((gauge_len - len_int) - 1.0))
            .draw(10, MyColor::Orange);
    }
}

void Button::update() {
    bool contains_cursor = contains(Cursor::Pos());

    if (contains_cursor and MouseL.down() and pressed_transition.isZero()) {
        for (auto cell_idx : step(std::size(path))) {
            auto [x, y] = path[cell_idx];
            if (rects[x][y]->contains(Cursor::Pos())) {
                start_index = cell_idx;
                break;
            }
        }
    }

    mouseover_transition.update(contains_cursor);
    pressed_transition.update(contains_cursor and MouseL.pressed());

    const double path_len = static_cast<double>(std::size(path));

    for (auto dist : step(std::size(path))) {
        auto cell_idx = (start_index + dist) % std::size(path);
        alpha_transitions[cell_idx].update(pressed_transition.value() >=
                                           (dist + 1) / path_len);
    }
}

bool Button::contains(Point pos) const {
    for (auto rect : rects) {
        if (rect.has_value() and rect->contains(pos)) {
            return true;
        }
    }
    return false;
}

bool Button::completed() const {
    auto path_len = std::size(path);
    return alpha_transitions[(start_index + path_len - 1) % path_len].isOne();
}

bool Button::is_filled(int32 x, int32 y) const {
    if (x < 0 or grid_size.x <= x or y < 0 or grid_size.y <= y) {
        return false;
    }
    return rects[x][y].has_value();
}
