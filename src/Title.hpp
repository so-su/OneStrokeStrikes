#pragma once
#include "Button.hpp"
#include "Common.hpp"
#include "DriftingPolyominoes.hpp"
#include "SimpleButton.hpp"

// タイトルシーン
class Title : public App::Scene {
   public:
    Title(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    // ブラウザを立ち上げる確認中の処理
    void confirm_update();

    // あそぶボタンの描画
    void draw_play_button() const;

    // ボタンの配列
    Array<Button> buttons;

    // ボタンのセルの大きさ
    const Array<int32> cell_sizes{90, 60, 60};

    // ボタンのグリッドの大きさの配列
    const Array<Size> grid_sizes{{7, 5}, {6, 3}, {6, 4}};

    // ボタンの一筆書きのパスの配列
    const Array<Array<Point>> paths{
        {
            {0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1}, {3, 0}, {4, 0},
            {4, 1}, {5, 1}, {6, 1}, {6, 2}, {5, 2}, {5, 3}, {4, 3},
            {4, 2}, {3, 2}, {3, 3}, {3, 4}, {2, 4}, {1, 4}, {1, 3},
            {2, 3}, {2, 2}, {1, 2}, {1, 1}, {0, 1},

        },
        {
            {0, 0},
            {0, 1},
            {0, 2},
            {1, 2},
            {2, 2},
            {3, 2},
            {3, 1},
            {4, 1},
            {4, 2},
            {5, 2},
            {5, 1},
            {5, 0},
            {4, 0},
            {3, 0},
            {2, 0},
            {2, 1},
            {1, 1},
            {1, 0},
        },
        {
            {2, 0},
            {3, 0},
            {3, 1},
            {3, 2},
            {4, 2},
            {5, 2},
            {5, 3},
            {4, 3},
            {3, 3},
            {2, 3},
            {1, 3},
            {0, 3},
            {0, 2},
            {0, 1},
            {1, 1},
            {1, 2},
            {2, 2},
            {2, 1},
        }};

    // ボタンの左上の座標の配列
    const Array<Point> upper_lefts{{190, 200}, {840, 230}, {780, 430}};

    // ボタンの色の配列
    const Array<Color> colors{MyColor::Red, MyColor::Green, MyColor::Blue};

    // ボタンの最小の透過率の配列
    const Array<double> alpha_mins{0.8, 0.8, 0.8};

    // 難易度変更ボタンの三角形
    Triangle left_triangle_small{370, 480, 30, -90_deg};
    Triangle left_triangle_large{370, 480, 60, -90_deg};
    Triangle right_triangle_small{630, 480, 30, 90_deg};
    Triangle right_triangle_large{630, 480, 60, 90_deg};

    // 難易度変更ボタンをクリックした時間
    double left_triangle_last_clicked_time{-1.0};
    double right_triangle_last_clicked_time{-1.0};

    // 画面下のメッセージウィンドウの長方形
    static constexpr Rect message_window{0, 700, 1400, 50};

    // ボタンがまだ押されていない
    bool can_press_button{true};

    // ブラウザを立ち上げる確認中か
    bool launch_browser_confirm{false};

    // 確認のウィンドウ
    static constexpr RoundRect confirm_window{400, 300, 600, 200, 30};

    // マスクの透過率
    Transition mask_alpha_transition{0.5s, 0.25s};

    // もどるボタン
    SimpleButton backward{Rect{580, 420, 100, 40}, MyColor::Backward,
                          MyColor::Orange, 5};

    // ひらくボタン
    SimpleButton open{Rect{720, 420, 100, 40}, MyColor::Forward,
                      MyColor::Orange, 5};

    // 背景に漂うポリオミノの描画を管理するオブジェクト
    DriftingPolyominoes drifting_polyominoes;
};
