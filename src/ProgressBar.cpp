#include "ProgressBar.hpp"

ProgressBar::ProgressBar(const Rect& outer_bar, Color color)
    : outer_bar(outer_bar.rounded(10)),
      inner_bar_full(outer_bar.stretched(-3)),
      inner_bar_mask(inner_bar_full.rounded(10).asPolygon()),
      color(color) {}

// バーの更新
void ProgressBar::update(double ratio_) {
    // 0.0〜1.0に収めて更新
    this->ratio = std::clamp(ratio_, 0.0, 1.0);

    // 型抜きされる前のバー
    const Rect inner_bar_rect{
        inner_bar_full.stretched(0, -inner_bar_full.w * (1.0 - ratio), 0, 0)};

    // マスクで型抜きすることで、実際に描画する内部のバーを得る
    if (auto polygons = Geometry2D::And(inner_bar_rect, inner_bar_mask);
        polygons.empty()) {
        inner_bar_polygon = none;
    } else {
        inner_bar_polygon = polygons.front();
    }
}

// バーの描画
void ProgressBar::draw() const {
    outer_bar.draw(ColorF{0.5});
    outer_bar.stretched(-3).draw(ColorF{0.2});  // 内部の背景
    if (inner_bar_polygon.has_value()) {
        inner_bar_polygon->draw(color);
    }
}
