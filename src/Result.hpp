#pragma once
#include "Common.hpp"
#include "SimpleButton.hpp"

// リザルトシーン
class Result : public App::Scene {
   public:
    Result(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    // 一筆書きの色の列をランレングス圧縮したときの平均の長さ
    double mean_run_length;
    
    // ランキングをみるボタン
    SimpleButton see_ranking{Rect{Arg::center = Point{550, 700}, 180, 50},
                             MyColor::Forward, MyColor::Orange, 5};

    // タイトルにもどるボタン
    SimpleButton back_to_title{Rect{Arg::center = Point{850, 700}, 180, 50},
                               MyColor::Backward, MyColor::Orange, 5};

    // まだボタンが押されていない
    bool can_press_button{true};

    // 枠
    static constexpr RoundRect frame{300, 160, 800, 470, 30};
    
    // 一番上の行を表示するy座標
    static constexpr int32 top{330};
    
    // 行の間隔
    static constexpr int32 line_space{50};
    
    // 行の左端
    static constexpr int32 left_center{380};
    
    // 行の右端
    static constexpr int32 right_center{1020};
};
