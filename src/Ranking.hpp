#pragma once
#include "Common.hpp"
#include "SimpleButton.hpp"

// ランキングシーン
class Ranking : public App::Scene {
   public:
    Ranking(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    // ユーザーid入力中の更新処理
    void input_mode_update();

    // ランキングを取得する
    bool get_ranking();

    // 取得済みのランキングを読み込む
    void load_ranking();

    // スコアを送信する
    bool send_score() const;

    // ランキングを描画する
    void draw_ranking() const;

    // 文字列がユーザーidとして有効かを返す
    static bool is_valid(const String& user_id);

    // ユーザーの情報をもつ構造体
    struct User {
        String user_id;
        uint32 score;
    };

    // ランキングをもつ配列
    Array<User> ranking;

    // ランキングの枠の配列
    std::array<Rect, 11> rects;

    // プレイヤーの順位
    Optional<int32> player_place{none};

    // ランキングの取得、登録に使うGoogle Apps ScriptのURL
    const URL url{
        U"https://script.google.com/macros/s/"
        U"AKfycbzaiBu8G7tMH6fxKK9iysd5wVD03pVLvqPbDZIq5ZzNveKAqA7DkdKWStjfwwXiY"
        U"73c/exec"};

    // ランキングを保存するファイル
    const FilePath save_file_path{U"ranking.json"};

    // ランキング登録ボタン
    SimpleButton ranking_register{Rect{1020, 670, 160, 50}, MyColor::Forward,
                                  MyColor::Orange, 5};

    // もどるボタン
    SimpleButton backward{Rect{1200, 670, 100, 50}, MyColor::Backward,
                          MyColor::Orange, 5};

    // ボタンがまだ押されていない
    bool can_press_button{true};

    // ランキングに登録できるかどうか
    bool display_register_button{false};

    // ユーザーidの入力中かどうか
    bool input_mode{false};

    // マスクの透過率
    Transition mask_alpha_transition{0.5s, 0.25s};

    // ユーザーid入力のウィンドウ
    static constexpr RoundRect window{400, 250, 600, 300, 30};

    // 文字の入力インターフェース
    // draw関数内で描画したいのでmutableをつける
    mutable TextEditState text_edit{U""};

    // スコア送信ボタン
    SimpleButton send{Rect{720, 460, 100, 40}, MyColor::Forward,
                      MyColor::Orange, 5};

    // 登録やめるボタン
    SimpleButton input_backward{Rect{580, 460, 100, 40}, MyColor::Backward,
                                MyColor::Orange, 5};
};
