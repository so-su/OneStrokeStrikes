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
    static constexpr Rect frame{300, 150, 800, 500};

    SimpleButton see_ranking{Rect{Arg::center = Point{550, 700}, 180, 50},
                             MyColor::Forward,MyColor::Orange, 5};

    SimpleButton back_to_title{Rect{Arg::center = Point{850, 700}, 180, 50},
                               MyColor::Backward, MyColor::Orange, 5};
    
    bool can_press_button{true};
};
