#pragma once
#include "Button.hpp"
#include "SimpleButton.hpp"
#include "Common.hpp"

// タイトルシーン
class Title : public App::Scene {
   public:
    Title(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    // ブラウザを立ち上げる確認中の処理
    void confirm_update();
    
    // ボタンの配列
    Array<Button> buttons;
    
    // ボタンのセルの大きさ
    static constexpr int32 cell_size{80};
    
    // ボタンのグリッドの大きさの配列
    const Array<Size> grid_sizes{
        {6, 2},
        {7, 5},
        {4, 3},
        {3, 2},
    };

    // ボタンの一筆書きのパスの配列
    const Array<Array<Point>> paths{
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

    // ボタンの左上の座標の配列
    const Array<Point> upper_lefts{
        {700, 200}, {150, 200}, {600, 400}, {970, 400}};

    // ボタンの色の配列
    const Array<Color> colors{MyColor::Green, MyColor::Red, MyColor::Blue,
                                 MyColor::Yellow};
    
    // ボタンの最小の透過率の配列
    const Array<double> alpha_mins{0.7, 0.7, 0.7, 0.5};

    // 画面下のメッセージウィンドウの長方形
    static constexpr Rect message_window{0, 700, 1400, 50};
    
    // ボタンがまだ押されていない
    bool can_press_button{true};
    
    // ブラウザを立ち上げる確認中か
    bool launch_browser_confirm{false};
    
    // 確認のウィンドウ
    static constexpr RoundRect confirm_window{400, 300, 600, 200, 30};
    
    // マスクの透過率
    Transition mask_alpha_transition{0.5s,0.25s};
    
    // もどるボタン
    SimpleButton backward{Rect{580, 420, 100, 40}, MyColor::Backward, MyColor::Orange, 5};

    // ひらくボタン
    SimpleButton open{Rect{720, 420, 100, 40}, MyColor::Forward, MyColor::Orange, 5};
};
