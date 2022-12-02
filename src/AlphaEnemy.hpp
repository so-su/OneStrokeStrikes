#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"
#include "ProgressBar.hpp"
#include "AttackShape.hpp"

class AlphaEnemy : public Polyomino {
   public:
    AlphaEnemy();
    
    void initialize();
    
    void new_shape_initialize();
    
    void get_damaged(size_t remove_num);
    
    void get_damaged(AttackShape* attack_shape);
    
    bool is_alive()const;
    
    // ゲージを更新し、満タンになったゲージの個数を返す
    int32 update_gauges(bool speed_up);
    
    // ゲージをリセットする
    void reset_gauge();
    
    // 外周のゲージを描画する
    void draw_gauges()const;
    
    // マウスオーバーで色を変化させる
    void mouse_over(Point pos);

   private:
    // 外周のパスの、各連結成分ごとの配列
    Array<Array<Line>> perimeters;
    
    // ゲージの長さの、各連結成分ごとの配列
    Array<double> gauge_lens;
    
    // 連結成分の番号
    Grid<Optional<int32>> component_id;
    
    // 連結成分の個数
    int32 num_component;
    
    // 名前
    String name;
    
    // 座標 pos にあるセルが属する連結成分の番号を返す
    Optional<int32> get_component_id(Point pos)const;
    
    // ゲージの色
    static constexpr Color gauge_color=MyColor::Orange;
};
