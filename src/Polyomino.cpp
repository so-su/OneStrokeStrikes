#include "Polyomino.hpp"

Polyomino::Polyomino(Size max_grid_size, Size cell_size, Point center)
    : cell_size(cell_size),
      center(center),
      cells{max_grid_size, Cell::None},
      rects{max_grid_size, none} {}

// ポリオミノを描画する
void Polyomino::draw() const {
    for (auto [i, j] : step(grid_size)) {
        if (cells[i][j] == Cell::None) continue;

        Color cell_color;
        if (cells[i][j] == Cell::Red) {
            cell_color = MyColor::Red;
        } else if (cells[i][j] == Cell::Blue) {
            cell_color = MyColor::Blue;
        } else if (cells[i][j] == Cell::Green) {
            cell_color = MyColor::Green;
        } else if (cells[i][j] == Cell::Yellow) {
            cell_color = MyColor::Yellow;
        } else if (cells[i][j] == Cell::Gray) {
            cell_color = ColorF{0.3};
        }
        rects[i][j]->draw(cell_color).drawFrame(2, 2, ColorF{0.25});
    }
}

// パスを描画する
void Polyomino::draw_path() const {
    if (path.empty()) {
        return;
    }

    for (size_t path_idx = 0; path_idx + 1 < size(path); ++path_idx) {
        Vec2 from = rects[path[path_idx].x][path[path_idx].y]->center();
        Vec2 to = rects[path[path_idx + 1].x][path[path_idx + 1].y]->center();
        Line{from, to}.draw(3, MyColor::Yellow);
    }

    Circle{rects[path.front().x][path.front().y]->center(), 5}.draw(MyColor::Yellow);

    if (size(path) >= 2) {
        Vec2 from =
            rects[next(std::rbegin(path))->x][next(std::rbegin(path))->y]
                ->center();
        Vec2 to = rects[path.back().x][path.back().y]->center();
        if (to.x < from.x - 0.5) {
            Triangle{to, 20, 270_deg}.draw(MyColor::Yellow);
        } else if (to.x > from.x + 0.5) {
            Triangle{to, 20, 90_deg}.draw(MyColor::Yellow);
        } else if (to.y < from.y - 0.5) {
            Triangle{to, 20, 0_deg}.draw(MyColor::Yellow);
        } else if (to.y > from.y + 0.5) {
            Triangle{to, 20, 180_deg}.draw(MyColor::Yellow);
        }
    }
}

// エフェクトを描画する
void Polyomino::draw_effect() const { effect.update(); }

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
                for (size_t path_idx = 0; path_idx < std::size(path); ++path_idx) {
                    if (path[path_idx] == Point{i, j}) {
                        // 末尾から2番目ならパスの末尾を削除
                        if (path_idx + 2 == std::size(path)) {
                            path.pop_back();
                            return true;
                        }
                        return false;
                    }
                }
            }
            path.emplace_back(i, j);
            // パスがポリオミノを覆ったならポリオミノの消滅が始まる
            if (std::size(path) == num_filled_cells) {
                vanishing_idx = 0;
                add_ring_effect();
            }
            return true;
        }
    }

    // posがポリオミノ外のとき
    return false;
}

// パスを削除する
void Polyomino::clear_path() { path.clear(); }

// パスを反転させる
void Polyomino::reverse_path() {
    std::reverse(std::begin(path), std::end(path));
}

// ポリオミノの消滅を進め、すべて消滅したらtrueを返す
bool Polyomino::vanish() {
    if (vanishing_idx == num_filled_cells) {
        vanished = true;
        return true;
    }

    auto [i, j] = path[*vanishing_idx];

    Optional<double> base_hue;
    if (cells[i][j] == Cell::Red) {
        base_hue = 0.0;
    } else if (cells[i][j] == Cell::Blue) {
        base_hue = 225.0;
    } else if (cells[i][j] == Cell::Green) {
        base_hue = 120.0;
    }
    if (base_hue.has_value()) {
        effect.add<SquareEffect>(rects[i][j]->center(), *base_hue);
    }

    cells[i][j] = Cell::None;
    ++(*vanishing_idx);

    return false;
}

// ポリオミノが消滅中かを返す
bool Polyomino::is_vanishing() const { return vanishing_idx.has_value(); }

// ポリオミノが完全に消滅したかを返す
bool Polyomino::has_vanished() const { return vanished; }

// セルが埋められているかを返す
bool Polyomino::is_filled(int32 x, int32 y) const {
    if (x < 0 or grid_size.x <= x or y < 0 or grid_size.y <= y) {
        return false;
    }
    return cells[x][y] != Cell::None;
}

// 倒したときにもらえる基礎スコアを計算
Score Polyomino::get_basic_score() const {
    Score score{0, 0, 0, 0, 0, 0};
    for (auto [i, j] : step(grid_size)) {
        if (cells[i][j] == Cell::Green) {
            score.green += 10;
        } else if (cells[i][j] == Cell::Red) {
            score.red += 10;
        } else if (cells[i][j] == Cell::Blue) {
            score.blue += 10;
        }
    }
    return score;
}

// 一筆書きのスコアを計算する
Score Polyomino::get_path_score() const {
    Score path_score{get_basic_score()};
    Cell now_color = cells[path.front().x][path.front().y];
    int32 cnt{1};
    for (size_t path_idx = 1; path_idx < size(path); ++path_idx) {
        if (cells[path[path_idx].x][path[path_idx].y] == now_color) {
            ++cnt;
        } else {
            // スコア計算
            if (now_color == Cell::Green) {
                path_score.green += cnt * cnt;
            } else if (now_color == Cell::Red) {
                path_score.red += cnt * cnt;
            } else if (now_color == Cell::Blue) {
                path_score.blue += cnt * cnt;
            }

            now_color = cells[path[path_idx].x][path[path_idx].y];
            cnt = 1;
        }
    }
    // 最後の連続部分のスコア計算
    if (now_color == Cell::Green) {
        path_score.green += cnt * cnt;
    } else if (now_color == Cell::Red) {
        path_score.red += cnt * cnt;
    } else if (now_color == Cell::Blue) {
        path_score.blue += cnt * cnt;
    }

    // 端点ボーナス
    if (cells[path.front().x][path.front().y] == Cell::Green) {
        ++path_score.green_bonus;
    } else if (cells[path.front().x][path.front().y] == Cell::Red) {
        ++path_score.red_bonus;
    } else if (cells[path.front().x][path.front().y] == Cell::Blue) {
        ++path_score.blue_bonus;
    }

    if (cells[path.back().x][path.back().y] == Cell::Green) {
        ++path_score.green_bonus;
    } else if (cells[path.back().x][path.back().y] == Cell::Red) {
        ++path_score.red_bonus;
    } else if (cells[path.back().x][path.back().y] == Cell::Blue) {
        ++path_score.blue_bonus;
    }

    return path_score;
}

// ランダムで消滅する準備をする
void Polyomino::prepare_to_randomly_vanish() {
    path = shuffled_filled_cells;
    vanishing_idx = 0;
}

// RingEffectを発生させる
void Polyomino::add_ring_effect() const {
    effect.add<RingEffect>(center, 40 * Max(grid_size.x, grid_size.y));
}

// ポリオミノを生成する
void Polyomino::generate_polyomino(Size grid_size_, const int32 tolerance,
                           Cell designated) {
    grid_size = grid_size_;
    cells.fill(Cell::None);
    rects.fill(none);
    num_filled_cells = 1;
    clear_path();
    vanishing_idx = none;
    vanished = false;
    shuffled_filled_cells.clear();

    // スタートする点をランダムに決め、セルを埋める
    int32 start_x = Random<int32>(grid_size.x - 1);
    int32 start_y = Random<int32>(grid_size.y - 1);
    cells[start_x][start_y] = generate_cell(designated);

    for (auto _ : step(2)) {
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
            ++num_filled_cells;

            prev = cells[now_x][now_y];
        }
    }

    // 余白を詰める
    resize();

    // ポリオミノの左上の座標を求める
    upper_left = center - cell_size.x * grid_size / 2;

    // rects と shuffled_filled_cells の初期化
    for (auto [i, j] : step(grid_size)) {
        if (not is_filled(i, j)) continue;
        rects[i][j] = Rect{upper_left.x + i * cell_size.x,
                           upper_left.y + j * cell_size.y, cell_size};
        shuffled_filled_cells.emplace_back(i, j);
    }

    shuffled_filled_cells.shuffle();
}

// 余白を詰める
void Polyomino::resize() {
    // 埋まっているセルのうち、最小、最大のxとy
    int32 min_x{grid_size.x},max_x{-1};
    int32 min_y{grid_size.y},max_y{-1};
    
    for (auto [x, y] : step(grid_size)) {
        if (not is_filled(x, y)) continue;
        min_x = Min(x, min_x);
        max_x = Max(x, max_x);
        min_y = Min(y, min_y);
        max_y = Max(y, max_y);
    }
    auto tmp{cells};
    cells.fill(Cell::None);

    grid_size =
        Size{max_x - min_x + 1, max_y - min_y + 1};
    for (auto [i, j] : step(grid_size)) {
        cells[i][j] = tmp[min_x + i][min_y + j];
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
