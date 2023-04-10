#include "Title.hpp"

Title::Title(const InitData& init) : IScene{init} {
    for (auto button_idx : step(std::size(paths))) {
        buttons.emplace_back(grid_sizes[button_idx], paths[button_idx],
                             cell_sizes[button_idx], upper_lefts[button_idx],
                             colors[button_idx], alpha_mins[button_idx]);
    }
}

void Title::update() {
    // マスクの透過率の更新
    mask_alpha_transition.update(launch_browser_confirm);

    // ブラウザを立ち上げる確認中のとき
    if (launch_browser_confirm) {
        confirm_update();
        return;
    }
    
    // 難易度変更をボタンの更新処理の前にやる
    if(left_triangle_large.leftClicked()) {
        if(auto& diff=getData().difficulty;diff!=Difficulty::Easy){
            if(diff==Difficulty::Normal){
                diff=Difficulty::Easy;
            }
            else if(diff==Difficulty::Hard){
                diff=Difficulty::Normal;
            }
        }
        return;
    }
    if(right_triangle_large.leftClicked()) {
        if(auto& diff=getData().difficulty;diff!=Difficulty::Hard){
            if(diff==Difficulty::Easy){
                diff=Difficulty::Normal;
            }
            else if(diff==Difficulty::Normal){
                diff=Difficulty::Hard;
            }
        }
        return;
    }

    // ボタンの更新
    for (auto& button : buttons) {
        if (button.update(can_press_button)) {
            can_press_button = false;
        }
    }

    if (buttons[0].completed()) {  // あそぶ
        changeScene(State::Game);
    } else if (buttons[1].completed()) {  // あそびかた
        launch_browser_confirm = true;
        buttons[1].reset();
        can_press_button = true;
    } else if (buttons[2].completed()) {  // ランキング
        changeScene(State::Ranking);
    }
}

void Title::draw() const {
    Scene::SetBackground(MyColor::White);

    FontAsset(U"Kaisotai")(U"すとすと").drawAt(700, 100, Palette::Black);

    // ボタンの描画
    for (const auto& button : buttons) {
        button.draw();
        button.draw_gauge();
    }

    FontAsset(U"Black")(U"あそぶ")
        .drawAt(TextStyle::Outline(0.5, Palette::Black), 80, 500, 350,
                ColorF{0.9});
    {
        String difficulty_text;
        if(getData().difficulty==Difficulty::Easy){
            difficulty_text=U"みならい";
        }else if(getData().difficulty==Difficulty::Normal){
            difficulty_text=U"じゅくれん";
        }
        else if(getData().difficulty==Difficulty::Hard){
            difficulty_text=U"しょくにん";
        }
        FontAsset(U"Black")(difficulty_text)
            .drawAt(TextStyle::Outline(0.5, Palette::Black), 40, 500, 450,
                    ColorF{0.9});
    }
    
    // 難易度変更の三角ボタン
    if(left_triangle_large.mouseOver()){
        left_triangle_large.draw();
    }
    else{
        left_triangle_small.draw();
    }
    if(right_triangle_large.mouseOver()){
        right_triangle_large.draw();
    }
    else{
        right_triangle_small.draw();
    }
    
    FontAsset(U"Black")(U"あそびかた").drawAt(
        TextStyle::Outline(0.5, Palette::Black), 50, 1020, 320, ColorF{0.9});
    FontAsset(U"Black")(U"ランキング")
        .drawAt(TextStyle::Outline(0.5, Palette::Black), 50, 960, 600,
                ColorF{0.9});
    
    // メッセージウィンドウの描画
    message_window.draw(ColorF{0.0, 0.4});
    if (buttons[0].contains(Cursor::Pos())) {
        FontAsset(U"Regular")(U"ドキドキブンレツ一筆書きパズル！")
            .drawAt(message_window.center(), Palette::White);
    } else if (buttons[1].contains(Cursor::Pos())) {
        FontAsset(U"Regular")(U"ルールをよむ")
            .drawAt(message_window.center(), Palette::White);
    } else if (buttons[2].contains(Cursor::Pos())) {
        FontAsset(U"Regular")(U"ランキングをみる")
            .drawAt(message_window.center(), Palette::White);
    }

    // マスクの描画
    Scene::Rect().draw(
        ColorF{Palette::Black, mask_alpha_transition.value() * 0.7});

    if (launch_browser_confirm) {
        confirm_window.draw(MyColor::White);

        FontAsset(U"Regular")(U"外部のウェブサイトを開きます")
            .drawAt(30, 700, 370, Palette::Black);

        // もどるボタン
        backward.draw();
        FontAsset(U"Regular")(U"もどる").drawAt(20, backward.center(),
                                                Palette::Black);

        // ひらくボタン
        open.draw();
        FontAsset(U"Regular")(U"ひらく").drawAt(20, open.center(),
                                                Palette::Black);
    }
}

// ブラウザを立ち上げる確認中の処理
void Title::confirm_update() {
    // ボタンの更新
    if (backward.update(can_press_button)) {
        can_press_button = false;
    }
    if (open.update(can_press_button)) {
        can_press_button = false;
    }

    // ボタンが押されていて、かつゲージが満タンの状態のときの処理
    if (backward.completed()) {
        launch_browser_confirm = false;
        can_press_button = true;
        backward.reset();
    } else if (open.completed()) {
        System::LaunchBrowser(U"https://github.com/so-su/OneStrokeStrikes");
        launch_browser_confirm = false;
        can_press_button = true;
        open.reset();
    }
}
