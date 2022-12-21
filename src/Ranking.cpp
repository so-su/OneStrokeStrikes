#include "Ranking.hpp"

Ranking::Ranking(const InitData& init) : IScene{init} {
    get_ranking();
    load_ranking();

    for (auto rank : step(11)) {
        rects[rank] = Rect{400, 170 + 50 * rank, 600, 50};
    }

    if (getData().display_player_score) {
        display_register_button = (std::size(ranking) < 10) or
                                  (getData().score > ranking.back().score);

        player_place = 10;
    } else {
        for (auto rank : step(std::size(ranking))) {
            const auto [user_id, score] = ranking[rank];
            if (user_id == getData().player_id and score == getData().score) {
                player_place = rank;
                break;
            }
        }
    }
}

void Ranking::update() {
    mask_alpha_transition.update(input_mode);

    if (input_mode) {
        send.update();
        input_backward.update();
        if (send.down()) {
            if (is_valid(text_edit.text)) {
                send_score();
                getData().display_player_score = false;
                getData().player_id = text_edit.text;
                changeScene(State::Ranking);
            }
        } else if (input_backward.down()) {
            input_mode = false;
        }
        return;
    }

    ranking_register.update();
    backward.update();

    if (ranking_register.down() and display_register_button) {
        input_mode = true;
    } else if (backward.down()) {
        changeScene(State::Title);
    }
}

void Ranking::draw() const {
    Scene::SetBackground(ColorF{0.9});

    FontAsset(U"Kaisotai")(U"ランキング").drawAt(80, 700, 100, Palette::Black);

    draw_ranking();

    if (display_register_button) {
        ranking_register.draw();
        FontAsset(U"Regular")(U"ランキング登録")
            .drawAt(20, ranking_register.center(), Palette::Black);
    }

    backward.draw();
    FontAsset(U"Regular")(U"もどる").drawAt(20, backward.center(),
                                            Palette::Black);

    if (input_mode) {
        Scene::Rect().draw(
            ColorF{Palette::Black, mask_alpha_transition.value() * 0.6});
        round_rect.draw(ColorF{0.9});
        FontAsset(U"Regular")(U"ニックネームを入力してください")
            .drawAt(700, 310, Palette::Black);
        FontAsset(U"Regular")(
            U"3文字以上16文字以下で、半角英数字, -, _ "
            U"が使えます。\nまた、アルファベットから始まる必要があります。")
            .drawAt(15, 700, 360, Palette::Dimgray);

        // テキストボックスを描画
        SimpleGUI::TextBox(text_edit, Vec2{500, 400}, 400, 16);

        if (is_valid(text_edit.text)) {
            Rect{580, 460, 100, 40}.draw(ColorF{MyColor::Orange, 0.2});
        }

        send.draw();
        FontAsset(U"Regular")(U"登録").drawAt(20, send.center(),
                                              Palette::Black);

        input_backward.draw();
        FontAsset(U"Regular")(U"もどる").drawAt(20, input_backward.center(),
                                                Palette::Black);
    }
}

bool Ranking::get_ranking() {
    if (SimpleHTTP::Get(url, {}, save_file_path)) {
        return true;
    }
    return false;
}

void Ranking::load_ranking() {
    ranking.clear();
    const JSON users = JSON::Load(U"ranking.json");
    for (const auto& user : users.arrayView()) {
        ranking.emplace_back(
            User{user[U"user_id"].get<String>(), user[U"score"].get<uint32>()});
    }
}

bool Ranking::send_score() const {
    const std::string data =
        (U"user_id={}&score={}"_fmt(text_edit.text, getData().score)).toUTF8();
    if (SimpleHTTP::Post(url, {}, data.data(), data.size(), save_file_path)) {
        return true;
    }
    return false;
}

void Ranking::draw_ranking() const {
    for (auto rank : step(10)) {
        rects[rank].drawFrame(1, 1, Palette::Dimgray);
        FontAsset(U"Kaisotai")(rank + 1).draw(
            40, Arg::rightCenter = Vec2{450, 170 + 50 * rank + 25},
            Palette::Black);
    }

    for (auto rank : step(std::size(ranking))) {
        const auto user = ranking[rank];
        FontAsset(U"Regular")(user.user_id)
            .draw(Arg::leftCenter = Vec2{500, 170 + 50 * rank + 25},
                  Palette::Black);
        FontAsset(U"Regular")(user.score)
            .draw(Arg::rightCenter = Vec2{980, 170 + 50 * rank + 25},
                  Palette::Black);
    }

    if (player_place.has_value()) {
        Triangle{370, static_cast<double>(195 + 50 * player_place.value()), 30,
                 90_deg}
            .moveBy(7 * Math::Sin(5 * Scene::Time()), 0)
            .draw(Palette::Black);
    }

    if (getData().display_player_score) {
        rects[10].drawFrame(1, 1, Palette::Dimgray);
        FontAsset(U"Regular")(U"あなたのスコア")
            .draw(Arg::leftCenter = Vec2{500, 170 + 50 * 10 + 25},
                  Palette::Black);
        FontAsset(U"Regular")(getData().score)
            .draw(Arg::rightCenter = Vec2{980, 170 + 50 * 10 + 25},
                  Palette::Black);
    }
}

bool Ranking::is_valid(const String& user_id) {
    return std::size(user_id) >= 3 and IsAlpha(user_id.front()) and
           user_id.all([](char32 c) -> bool {
               return IsAlnum(c) or c == '-' or c == '_';
           });
}
