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

// エフェクトを描画する
void Polyomino::draw_effect() const { effect.update(); }

// ポリオミノの消滅を進め、消滅させるセルがなくなったらtrueを返す
bool Polyomino::vanish() {
    // 削除するセルがないとき
    if (cells_to_erase.empty()) {
        return true;
    }

    const auto pos{cells_to_erase.back()};
    cells_to_erase.pop_back();

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
    --num_filled_cells;

    return cells_to_erase.empty();
}

// ポリオミノが消滅中かを返す
bool Polyomino::is_vanishing() const { return not cells_to_erase.empty(); }

// 埋まっているセルがあるかを返す
bool Polyomino::is_alive() const { return num_filled_cells > 0; }

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
