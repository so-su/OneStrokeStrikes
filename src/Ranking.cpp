#include "Ranking.hpp"

Ranking::Ranking(const InitData& init) : IScene{init} {
    get_task = SimpleHTTP::SaveAsync(url, save_file_path);
}

void Ranking::update() {
    if (KeyEscape.down()) {
        changeScene(State::Title);
    }

    if (get_task.isReady() and ranking.empty()) {
        load_ranking();
    }

    timer += Scene::DeltaTime();
    if (not get_task.isReady()) {  // まだランキングの準備中
        timer = fmod(timer, num_squares * (rot_duration - duration_overlap) +
                                duration_overlap);
    } else {  // ランキングの準備をする非同期処理が終わった
        timer = 0.0;
    }

    mask_alpha_transition.update(input_mode);

    // ユーザーidの入力中
    if (input_mode) {
        input_mode_update();
        return;
    }

    // ボタンの更新
    if (ranking_register.update(can_press_button)) {
        can_press_button = false;
    }
    if (backward.update(can_press_button)) {
        can_press_button = false;
    }

    // ボタンが押されていて、かつゲージが満タンのときの処理
    if (ranking_register.completed() and display_register_button) {
        ranking_register.reset();
        input_mode = true;
        can_press_button = true;
    } else if (backward.completed()) {
        changeScene(State::Title);
    }
}

void Ranking::draw() const {
    Scene::SetBackground(MyColor::Background);

    FontAsset(U"Kaisotai")(U"ランキング").drawAt(80, 700, 100, MyColor::White);

    // もどるボタン
    backward.draw();
    FontAsset(U"Regular")(U"もどる").drawAt(20, backward.center(),
                                            Palette::Black);

    if (not get_task.isReady()) {
        draw_loading_animation();
        return;
    }

    draw_ranking();

    // ランキングに登録できるとき、ランキング登録ボタンを表示
    if (display_register_button) {
        ranking_register.draw();
        FontAsset(U"Regular")(U"ランキング登録")
            .drawAt(20, ranking_register.center(), Palette::Black);
    }

    // ユーザーid入力中
    if (input_mode) {
        // マスクを描画
        Scene::Rect().draw(
            ColorF{MyColor::Background, mask_alpha_transition.value() * 0.6});

        window.draw(MyColor::White);

        FontAsset(U"Regular")(U"ニックネームを入力してください")
            .drawAt(700, 310, Palette::Black);
        FontAsset(U"Regular")(
            U"3文字以上12文字以下で、半角英数字と3種の記号（- _ "
            U".）が使えます。\n"
            U"ただし、アルファベットから始まる必要があります。")
            .drawAt(15, 700, 360, Palette::Dimgray);

        // ユーザーidを入力するテキストボックス
        SimpleGUI::TextBox(text_edit, Vec2{500, 400}, 400, 12);

        // スコア送信ボタン
        send.draw();
        FontAsset(U"Regular")(U"登録").drawAt(20, send.center(),
                                              Palette::Black);

        // 登録やめるボタン
        input_backward.draw();
        FontAsset(U"Regular")(U"もどる").drawAt(20, input_backward.center(),
                                                Palette::Black);
    }
}

// ユーザーid入力中の更新処理
void Ranking::input_mode_update() {
    // ボタンが押されて、かつゲージが満タンのときの処理
    // 後述の更新処理を先に書くと、send_score()が呼ばれたときに
    // その処理が終わってdraw()関数が呼ばれるまで、ゲージの進み具合が更新されないことがある
    if (send.completed()) {
        if (is_valid(text_edit.text)) {
            send_score();
            getData().display_player_score = false;
            getData().player_id = text_edit.text;
            changeScene(State::Ranking);
        }
    } else if (input_backward.completed()) {
        input_backward.reset();
        input_mode = false;
        can_press_button = true;
    }

    // ボタンの更新
    if (send.update(can_press_button and is_valid(text_edit.text))) {
        can_press_button = false;
    }
    if (input_backward.update(can_press_button)) {
        can_press_button = false;
    }

    // 有効なユーザーidのときは、スコア送信ボタンの色を変える
    if (is_valid(text_edit.text)) {
        send.set_inner_color(MyColor::Forward);
    } else {
        send.set_inner_color(MyColor::Backward);
    }
}

// 取得済みのランキングを読み込む
void Ranking::load_ranking() {
    // jsonファイルからランキング配列を構成する
    ranking.clear();
    const JSON users{JSON::Load(save_file_path)};
    for (const auto& user : users.arrayView()) {
        ranking.emplace_back(
            User{user[U"user_id"].get<String>(), user[U"score"].get<int32>()});
    }

    // 各順位の枠
    // 11個目は自分のスコアが入る
    for (auto rank : step(11)) {
        rects[rank] = Rect{400, 170 + 50 * rank, 600, 50};
    }

    if (getData().display_player_score) {  // プレイヤーのスコアが表示できるとき
        // ランキングに登録できるかを判定する
        display_register_button = (std::size(ranking) < 10) or
                                  (getData().score > ranking.back().score);
        // 11位の位置にプレイヤーのスコアを表示
        player_place = 10;
    } else {  // プレイヤーのスコアが表示できないとき
        // もしプレイヤーがランクインしていたら、player_placeを設定
        for (auto rank : step(std::size(ranking))) {
            const auto [user_id, score] = ranking[rank];
            if (user_id == getData().player_id and score == getData().score) {
                player_place = rank;
                break;
            }
        }
    }
}

// スコアを送信する
bool Ranking::send_score() const {
    const std::string data =
        (U"user_id={}&score={}"_fmt(text_edit.text, getData().score)).toUTF8();
    if (SimpleHTTP::Post(url, {}, data.data(), data.size(), save_file_path)) {
        return true;
    }
    return false;
}

// ローディング中のアニメーションを描画する
void Ranking::draw_loading_animation() const {
    constexpr int32 square_size{30};
    constexpr int32 center_spacing{40};

    for (int32 square_idx = 0; square_idx < num_squares; ++square_idx) {
        const Point center{
            Scene::Width() / 2 +
                center_spacing / 2 * (square_idx * 2 - num_squares + 1),
            Scene::Height() / 2};
        const double arg{
            EaseOutBounce(std::clamp(
                (timer - (rot_duration - duration_overlap) * square_idx) /
                    rot_duration,
                0.0, 1.0)) *
            Math::HalfPi};
        Rect{Arg::center = center, square_size, square_size}
            .rotatedAt(center, arg)
            .draw(ColorF{1.0, 0.5});
    }
}

// ランキングを描画する
void Ranking::draw_ranking() const {
    for (auto rank : step(10)) {
        rects[rank].drawFrame(1, 1, Palette::Dimgray);
        FontAsset(U"Kaisotai")(rank + 1).draw(
            40, Arg::rightCenter = Vec2{450, 170 + 50 * rank + 25},
            MyColor::White);
    }

    for (auto rank : step(std::size(ranking))) {
        const auto& user{ranking[rank]};
        FontAsset(U"Regular")(user.user_id)
            .draw(Arg::leftCenter = Vec2{500, 170 + 50 * rank + 25},
                  MyColor::White);
        FontAsset(U"Regular")(user.score)
            .draw(Arg::rightCenter = Vec2{980, 170 + 50 * rank + 25},
                  MyColor::White);
    }

    // プレイヤーの位置の左で三角形を左右に動かす
    if (player_place.has_value()) {
        Triangle{370, static_cast<double>(195 + 50 * player_place.value()), 30,
                 90_deg}
            .moveBy(7 * Math::Sin(5 * Scene::Time()), 0)
            .draw(MyColor::White);
    }

    // プレイヤーのスコアを表示
    if (getData().display_player_score) {
        rects[10].drawFrame(1, 1, Palette::Dimgray);
        FontAsset(U"Regular")(U"あなたのスコア")
            .draw(Arg::leftCenter = Vec2{500, 170 + 50 * 10 + 25},
                  MyColor::White);
        FontAsset(U"Regular")(getData().score)
            .draw(Arg::rightCenter = Vec2{980, 170 + 50 * 10 + 25},
                  MyColor::White);
    }
}

// 文字列がユーザーidとして有効かを返す
bool Ranking::is_valid(const String& user_id) {
    // 英数字または'-','_','.'からなる、3文字以上の文字列ならOK
    // ただしアルファベットから始まらないとNG
    return std::size(user_id) >= 3 and IsAlpha(user_id.front()) and
           user_id.all([](char32 c) -> bool {
               return IsAlnum(c) or c == '-' or c == '_' or c == '.';
           });
}
