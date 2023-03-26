#pragma once
#include "Common.hpp"
#include "ProgressBar.hpp"

class Player {
   public:
    Player();

    // ダメージを受ける
    void get_damaged(int32 value);

    // HPを回復する
    void get_healed(int32 value);

    // APを得る
    void get_ap(int32 value);

    // SPを得る
    void get_sp(int32 value);

    // 生きているかを返す
    bool is_alive() const;

    // APが満タンかを返す
    bool ap_is_full() const;

    // SPが満タンかを返す
    bool sp_is_full() const;

    // APを0にする
    void reset_ap();

    // SPを0にする
    void reset_sp();

    // HP,AP,SPのバーを描画する
    void draw() const;
    
    // HPバーを描画する
    void draw_hp_bar() const;
    
    // APバーを描画する
    void draw_ap_bar() const;
    
    // SPバーを描画する
    void draw_sp_bar() const;

   private:
    // 現在のHP
    int32 hp{max_hp};

    // 現在のAP
    int32 ap{0};

    // 現在のSP
    int32 sp{0};

    // 最大のHP
    static constexpr int32 max_hp{1000};

    // 最大のAP
    static constexpr int32 max_ap{1000};

    // 最大のSP
    static constexpr int32 max_sp{1500};

    // HPのバー
    ProgressBar hp_bar{Rect{100, 750, 800, 20}, MyColor::Green};

    // APのバー
    ProgressBar ap_bar{Rect{950, 750, 100, 20}, MyColor::Red};

    // SPのバー
    ProgressBar sp_bar{Rect{1100, 750, 100, 20}, MyColor::Blue};
};
