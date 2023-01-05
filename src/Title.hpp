#pragma once
#include "Button.hpp"
#include "Common.hpp"

// タイトルシーン
class Title : public App::Scene {
   public:
    Title(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    Array<Button> buttons;

    bool pause{false};
    
    Transition alpha_transition{1.0s,0.5s};

    static constexpr int32 cell_size = 80;

    const Array<Array<Point>> paths = {
        {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {5, 1},
            {4, 1},
            {3, 1},
            {2, 1},
            {1, 1},
            {0, 1},
        },
        {
            {2, 2}, {3, 2}, {3, 3}, {4, 3}, {4, 2}, {5, 2}, {5, 1},
            {4, 1}, {3, 1}, {2, 1}, {2, 0}, {1, 0}, {0, 0}, {0, 1},
            {1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 4}, {2, 3},
        },
        {
            {1, 0},
            {2, 0},
            {2, 1},
            {3, 1},
            {3, 2},
            {2, 2},
            {1, 2},
            {0, 2},
            {0, 1},
            {1, 1},
        },
        {
            {0, 0},
            {0, 1},
            {1, 1},
            {2, 1},
            {2, 0},
            {1, 0},
        }};

    const Array<Size> grid_sizes = {
        {6, 2},
        {7, 5},
        {4, 3},
        {3, 2},
    };

    const Array<Point> upper_lefts = {
        {700, 200}, {150, 200}, {600, 400}, {970, 400}};

    const Array<Color> colors = {MyColor::Green, MyColor::Red, MyColor::Blue,
                                 MyColor::Yellow};

    static constexpr Rect message_window = Rect{50, 700, 1300, 50};
};
