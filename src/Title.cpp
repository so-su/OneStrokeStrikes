#include "Title.hpp"

Title::Title(const InitData& init) : IScene{init} {
    for (auto button_idx : step(std::size(paths))) {
        buttons.emplace_back(grid_sizes[button_idx],
                             paths[button_idx],
                             cell_size, upper_lefts[button_idx],
                             colors[button_idx],
                             alpha_mins[button_idx]);
    }
}

void Title::update() {
    // マスクの透過率の更新
    mask_alpha_transition.update(launch_browser_confirm);
    
    // ブラウザを立ち上げる確認中のとき
    if(launch_browser_confirm){
        confirm_update();
        return;
    }
    
    // ボタンの更新
    for (auto& button : buttons) {
        if(button.update(can_press_button)){
            can_press_button = false;
        }
    }
    
    if (buttons[0].completed()) { // ちょっとあそぶ
        getData().easy_mode = true;
        getData().win = 0;
        changeScene(State::Game, 4000);
    }
    else if (buttons[1].completed()) { // あそぶ
        getData().easy_mode = false;
        getData().win = 0;
        changeScene(State::Game, 4000);
    }
    else if (buttons[2].completed()) { // ランキング
        changeScene(State::Ranking, 4000);
    }
    else if (buttons[3].completed()) { // あそびかた
        launch_browser_confirm = true;
        buttons[3].reset();
        can_press_button = true;
        
    }
}

// ブラウザを立ち上げる確認中の処理
void Title::confirm_update(){
    // ボタンの更新
    if(backward.update(can_press_button)){
        can_press_button = false;
    }
    if(open.update(can_press_button)){
        can_press_button = false;
    }
    
    // ボタンが押されていて、かつゲージが満タンの状態のときの処理
    if(backward.completed()){
        launch_browser_confirm = false;
        can_press_button = true;
        backward.reset();
    }
    else if(open.completed()){
        System::LaunchBrowser(U"https://github.com/so-su/OneStrokeStrikes");
        launch_browser_confirm = false;
        can_press_button = true;
        open.reset();
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

    FontAsset(U"Black")(U"ちょっとあそぶ")
        .drawAt(TextStyle::Outline(0.5, Palette::Black), 50, 940, 280,
                ColorF{0.9});
    FontAsset(U"Black")(U"あそぶ").drawAt(
        TextStyle::Outline(0.5, Palette::Black), 390, 380, ColorF{0.9});
    FontAsset(U"Black")(U"ランキング")
        .drawAt(TextStyle::Outline(0.5, Palette::Black), 40, 760, 560,
                ColorF{0.9});
    FontAsset(U"Black")(U"あそびかた")
        .drawAt(TextStyle::Outline(0.5, Palette::Black), 40, 1090, 480,
                ColorF{0.9});

    // メッセージウィンドウの描画
    message_window.draw(ColorF{0.0, 0.4});
    if (buttons[0].contains(Cursor::Pos())) {
        FontAsset(U"Regular")(U"はじめてのかたはこちらから")
            .drawAt(message_window.center(), Palette::White);
    } else if (buttons[1].contains(Cursor::Pos())) {
        FontAsset(U"Regular")(U"連勝めざしてひたすら一筆書き！")
            .drawAt(message_window.center(), Palette::White);
    } else if (buttons[2].contains(Cursor::Pos())) {
        FontAsset(U"Regular")(U"ランキングをみる")
            .drawAt(message_window.center(), Palette::White);
    } else if (buttons[3].contains(Cursor::Pos())) {
        FontAsset(U"Regular")(U"ルールをよむ")
            .drawAt(message_window.center(), Palette::White);
    }
    
    // マスクの描画
    Scene::Rect().draw(ColorF{Palette::Black, mask_alpha_transition.value() * 0.7});
    
    if(launch_browser_confirm){
        confirm_window.draw(MyColor::White);
        
        FontAsset(U"Regular")(U"外部のウェブサイトを開きます").drawAt(30,700,370 , Palette::Black);
        
        // もどるボタン
        backward.draw();
        FontAsset(U"Regular")(U"もどる").drawAt(20, backward.center(), Palette::Black);
        
        // ひらくボタン
        open.draw();
        FontAsset(U"Regular")(U"ひらく").drawAt(20, open.center(), Palette::Black);
    }
}
