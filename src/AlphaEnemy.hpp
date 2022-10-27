#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"
#include "ProgressBar.hpp"

class AlphaEnemy : public Polyomino {
   public:
    AlphaEnemy();
    
    void initialize();
    
    void new_shape_initialize();
    
    void get_damaged(size_t remove_num);
    
    void get_damaged(size_t remove_num,Point pos);
    
    bool alive()const;
    
    int32 attack_value()const;
    
    // ゲージを更新し、満タンになったゲージの個数を返す
    int32 update_gauges(bool speed_up);
    
    // 外周のゲージを描画する
    void draw_gauges()const;

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
};
