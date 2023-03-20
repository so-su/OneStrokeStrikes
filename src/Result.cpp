#include "Result.hpp"

Result::Result(const InitData& init) : IScene{init} {
    // スコアを計算する
    getData().score = 100;
    // 100 * getData().win * (getData().win + 1) / 2 + getData().enemy;
}

void Result::update() {
    // easyモードのとき
    if (not getData().easy_mode) {
        if (see_ranking.update(can_press_button)) {
            can_press_button = false;
        }

        // ランキングをみるボタンが押されていて、かつゲージが満タンの状態のとき
        if (see_ranking.completed()) {
            getData().display_player_score = true;
            changeScene(State::Ranking);
        }
    }

    if (back_to_title.update(can_press_button)) {
        can_press_button = true;
    }

    // タイトルにもどるボタンが押されていて、かつゲージが満タンの状態のとき
    if (back_to_title.completed()) {
        getData().display_player_score = false;
        changeScene(State::Title);
    }
}

void Result::draw() const {
    Scene::SetBackground(MyColor::White);

    FontAsset(U"Kaisotai")(U"リザルト").drawAt(80, 700, 100, Palette::Black);
    frame.drawFrame(1, 1, Palette::Dimgray);

    // easyモードのとき
    if (getData().easy_mode) {
        if (getData().win > 0) {
            FontAsset(U"Black")(U"あなたの勝ち！")
                .drawAt(50, Scene::Center(), Palette::Black);
        } else {
            FontAsset(U"Black")(U"あなたの負け...")
                .drawAt(50, Scene::Center(), Palette::Black);
        }
    } else {  // easyモードじゃないとき
        FontAsset(U"Black")(U"連勝数 {}"_fmt(getData().win))
            .drawAt(50, 700, 350, Palette::Black);
        FontAsset(U"Black")(U"スコア {}"_fmt(getData().score))
            .drawAt(50, 700, 430, Palette::Black);
    }

    if (not getData().easy_mode) {
        see_ranking.draw();
        FontAsset(U"Regular")(U"ランキングをみる")
            .drawAt(20, 550, 700, Palette::Black);
    }

    back_to_title.draw();
    FontAsset(U"Regular")(U"タイトルにもどる")
        .drawAt(20, 850, 700, Palette::Black);
}
