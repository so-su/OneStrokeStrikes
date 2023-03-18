#pragma once
#include "AttackShape.hpp"
#include "Common.hpp"
#include "Polyomino.hpp"
#include "ProgressBar.hpp"

class AlphaEnemy : public Polyomino {
   public:
    AlphaEnemy(bool easy);

    // 初期化する
    void initialize();

    // 各連結成分の外周を計算する
    void compute_perimeters();

    // 埋まっているセルを、指定した個数だけ削除する
    void get_damaged(size_t remove_num);

    // 埋まっているセルのうち、指定した図形と重なるものを削除する
    bool get_damaged(AttackShape* attack_shape);

    // ゲージを更新し、満タンになったゲージの個数を返す
    int32 update_gauges();

    // ゲージをリセットする
    void reset_gauge();

    // ゲージを描画する
    void draw_gauges() const;
    
    // セルの大きさ
    static constexpr int32 cell_size{30};

    // マウスオーバーで色を変化させる
    //void mouse_over(Point pos);

   private:
    
    // 外周のパスの、各連結成分ごとの配列
    Array<Array<Line>> perimeters;

    // ゲージの長さの、各連結成分ごとの配列
    Array<double> gauge_lens;

    // 連結成分の番号
    Grid<Optional<int32>> component_id{20, 8, none};

    // 連結成分の個数
    int32 num_components;

    // ゲージの色
    static constexpr Color gauge_color{MyColor::Orange};
    
    // 与えられた座標にあるセルが属する連結成分の番号を返す
    //Optional<int32> get_component_id(Point pos) const;
};
