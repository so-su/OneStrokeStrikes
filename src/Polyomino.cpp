#include "Polyomino.hpp"

Polyomino::Polyomino(Size max_grid_size, int32 cell_size, Point center)
    : cell_size(cell_size),
      center(center),
      cells{max_grid_size, Cell::None},
      rects{max_grid_size, none} {}

// ポリオミノを描画する
void Polyomino::draw() const {
    for(auto pos : step(grid_size)){
        if (not is_filled(pos)) continue;
        
        Color cell_color;
        if (cells[pos] == Cell::Red) {
            cell_color = MyColor::Red;
        } else if (cells[pos] == Cell::Blue) {
            cell_color = MyColor::Blue;
        } else if (cells[pos] == Cell::Green) {
            cell_color = MyColor::Green;
        } else if (cells[pos] == Cell::Yellow) {
            cell_color = MyColor::Yellow;
        }
        
        rects[pos]->draw(cell_color).drawFrame(2, 2, ColorF{0.25});
    }
}

// パスを描画する
void Polyomino::draw_path() const {
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

// エフェクトを描画する
void Polyomino::draw_effect() const { effect.update(); }

// カーソルの座標を受け取ってパスを更新
void Polyomino::update_path(Point cursor_pos) {
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
                vanishing_idx = 0;
                add_ring_effect();
            }
            
            return;
        }
    }
    
    /* cursor_posがポリオミノ外にあるとき、ここに到達する */
}

// パスを削除する
void Polyomino::clear_path() { path.clear(); }

// パスを反転させる
void Polyomino::reverse_path() {
    std::reverse(std::begin(path), std::end(path));
}

// ポリオミノの消滅を進め、すべて消滅したらtrueを返す
bool Polyomino::vanish() {
    // すべて消滅したとき
    if (vanishing_idx == num_filled_cells) {
        vanished = true;
        return true;
    }

    const auto pos{path[*vanishing_idx]};

    // SquareEffectを発生させる
    Optional<double> base_hue;
    if (cells[pos] == Cell::Red) {
        base_hue = 0.0;
    } else if (cells[pos] == Cell::Blue) {
        base_hue = 225.0;
    } else if (cells[pos] == Cell::Green) {
        base_hue = 120.0;
    }
    if (base_hue.has_value()) {
        effect.add<SquareEffect>(rects[pos]->center(), *base_hue);
    }

    // 埋められたセルをひとつ消滅させる
    cells[pos] = Cell::None;
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
    return cells[y][x] != Cell::None;
}

// セルが埋められているかを返す
bool Polyomino::is_filled(Point pos) const{
    return is_filled(pos.x, pos.y);
}

// 倒したときにもらえる基礎スコアを計算
Score Polyomino::get_basic_score() const {
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
Score Polyomino::get_path_score() const {
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

// ランダムで消滅する準備をする
void Polyomino::prepare_to_randomly_vanish() {
    path = shuffled_filled_cells;
    vanishing_idx = 0;
}

// RingEffectを発生させる
void Polyomino::add_ring_effect() const {
    const int32 effect_size{35 * Max(grid_size.x, grid_size.y)};
    effect.add<RingEffect>(center, effect_size);
}

// ポリオミノを初期化する
void Polyomino::initialize(Size grid_size_, Cell designated) {
    grid_size = grid_size_;
    cells.fill(Cell::None);
    rects.fill(none);
    num_filled_cells = 1;
    clear_path();
    vanishing_idx = none;
    vanished = false;
    shuffled_filled_cells.clear();

    generate_polyomino(designated);

    shrink_to_fit();

    upper_left = center - cell_size * grid_size / 2;

    for (auto pos : step(grid_size)) {
        if (not is_filled(pos)) continue;
        rects[pos] = Rect{upper_left + cell_size * pos, cell_size};
        shuffled_filled_cells.emplace_back(pos);
    }
    shuffled_filled_cells.shuffle();
}

// 一筆書き可能なポリオミノを生成する
void Polyomino::generate_polyomino(Cell designated) {
    // スタートする点をランダムに決め、セルを埋める
    const Point start{Random<int32>(grid_size.x - 1),Random<int32>(grid_size.y - 1)};
    cells[start] = generate_cell(designated);

    // 4方向を表す配列
    static constexpr std::array<Point, 4> directions{
        Point{1, 0}, Point{0, 1}, Point{-1, 0}, Point{0, -1}};
    
    const int32 tolerance{grid_size.x * grid_size.y};
    
    // 2方向に伸ばすことで、極端な図形になるのを防止する
    for (auto _ : step(2)) {
        Point now{start};
        Cell prev{cells[start]};
        int32 error_cnt{0};
        
        // ランダムな方向に進みながらセルを埋め、ポリオミノをつくる
        while (error_cnt < tolerance) {
            const auto dir_idx = Random<size_t>(3);
            const auto next{now + directions[dir_idx]};

            if (next.x < 0 or grid_size.x <= next.x or next.y < 0 or
                grid_size.y <= next.y or is_filled(next)) {
                ++error_cnt;
                continue;
            }

            now = next;

            // 50%の確率で前のセルと同じ色にする
            if (RandomBool()) {
                cells[now] = generate_cell(designated);
            } else {
                cells[now] = prev;
            }
            ++num_filled_cells;

            prev = cells[now];
        }
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

// ポリオミノをグリッドの左上に寄せてgrid_sizeを詰める
void Polyomino::shrink_to_fit() {
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
    
    const auto tmp{cells};
    cells.fill(Cell::None);
    grid_size =
        Size{max_x - min_x + 1, max_y - min_y + 1};
    for (auto [x, y] : step(grid_size)) {
        cells[y][x] = tmp[min_y + y][min_x + x];
    }
}
