#pragma once
#include "Common.hpp"
#include "ProgressBar.hpp"

class Player {
   public:
    Player();
    
    // 遅延つきHP、AP、SPを更新する
    void update_delay_status();

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
    // 現在のHP、AP、SP
    int32 hp{max_hp};
    int32 ap{0};
    int32 sp{0};

    // 最大のHP、AP、SP
    static constexpr int32 max_hp{1000};
    static constexpr int32 max_ap{1000};
    static constexpr int32 max_sp{1500};
    
    // 遅延つきHP、AP、SP
    double delay_hp{static_cast<double>(max_hp)};
    double delay_ap{0.0};
    double delay_sp{0.0};
    
    // 遅延つきHP、AP、SPの変化速度
    double delay_velocity_hp{0.0};
    double delay_velocity_ap{0.0};
    double delay_velocity_sp{0.0};
    
    // 遅延つきステータスの平滑化時間（秒）
    static constexpr double smooth_time{0.1};

    // HP、AP、SPのバー
    ProgressBar hp_bar{Rect{100, 750, 800, 20}, MyColor::Green};
    ProgressBar ap_bar{Rect{950, 750, 100, 20}, MyColor::Red};
    ProgressBar sp_bar{Rect{1100, 750, 100, 20}, MyColor::Blue};
};
