#pragma once
#include "Common.hpp"

class AttackShape {
   public:
    AttackShape();

    // ランダムに図形を初期化する
    void initialize();

    // 図形を描画する
    void draw(Point center, Color color) const;

    // 図形を表すグリッド
    Grid<bool> shape;
    
    // セルの一辺の長さ
    static constexpr int32 cell_size{30};

   private:
    // ランダムに図形を作るためにセルのインデックスを入れておく配列
    Array<Point> cell_pos;
};
