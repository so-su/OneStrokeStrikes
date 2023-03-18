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
    bool get_ranking();

    void load_ranking();

    bool send_score() const;

    void draw_ranking() const;

    static bool is_valid(const String& user_id);

    bool can_register() const;

    struct User {
        String user_id;
        uint32 score;
    };

    Array<User> ranking;

    std::array<Rect, 11> rects;

    Optional<int32> player_place{none};

    const URL url =
        U"https://script.google.com/macros/s/"
        U"AKfycbzaiBu8G7tMH6fxKK9iysd5wVD03pVLvqPbDZIq5ZzNveKAqA7DkdKWStjfwwXiY"
        U"73c/exec";

    const FilePath save_file_path = U"ranking.json";

    SimpleButton ranking_register{Rect{1020, 670, 160, 50}, MyColor::Forward, MyColor::Orange, 5};

    SimpleButton backward{Rect{1200, 670, 100, 50}, MyColor::Backward, MyColor::Orange, 5};

    bool input_mode{false};

    Transition mask_alpha_transition{0.5s, 0.5s};

    static constexpr RoundRect round_rect{400, 250, 600, 300, 30};

    mutable TextEditState text_edit{U""};

    SimpleButton send{Rect{720, 460, 100, 40}, MyColor::Forward ,MyColor::Orange, 5};

    SimpleButton input_backward{Rect{580, 460, 100, 40}, MyColor::Backward, MyColor::Orange, 5};

    bool display_register_button{false};
    
    // ボタンがまだ押されていない
    bool can_press_button{true};
};
