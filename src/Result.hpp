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
    // ランキングをみるボタン
    SimpleButton see_ranking{Rect{Arg::center = Point{550, 700}, 180, 50},
                             MyColor::Forward, MyColor::Orange, 5};

    // タイトルにもどるボタン
    SimpleButton back_to_title{Rect{Arg::center = Point{850, 700}, 180, 50},
                               MyColor::Backward, MyColor::Orange, 5};

    // まだボタンが押されていない
    bool can_press_button{true};

    // 枠
    static constexpr Rect frame{300, 150, 800, 500};
};
