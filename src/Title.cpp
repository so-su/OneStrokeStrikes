#include "Title.hpp"

Title::Title(const InitData& init) : IScene{init} {
    for(auto button_idx:step(std::size(paths))){
        buttons.emplace_back(paths[button_idx],grid_sizes[button_idx],cell_size,upper_lefts[button_idx],colors[button_idx]);
    }
    
    getData().score=Random(5,100);
}

void Title::update() {
    for(auto& button:buttons){
        button.update();
    }
    
    if(buttons[0].completed()){
        getData().easy_mode=true;
        changeScene(State::Game,4000);
    }
    if(buttons[1].completed()){
        getData().easy_mode=false;
        changeScene(State::Game,4000);
    }
    if(buttons[2].completed()){
        changeScene(State::Ranking,4000);
    }
    if(buttons[3].completed()){
        System::LaunchBrowser(U"https://github.com/so-su/OneStrokeStrikes");
    }
}

void Title::draw() const {
    Scene::SetBackground(ColorF{0.9});

    FontAsset(U"Kaisotai")(U"すとすと").drawAt(700,100,Palette::Black);

    for(const auto& button:buttons){
        button.draw();
        button.draw_gauge();
    }
    
    FontAsset(U"Black")(U"ちょっとあそぶ").drawAt(TextStyle::Outline(0.5,Palette::Black),50,940,280,ColorF{0.9});
    FontAsset(U"Black")(U"あそぶ").drawAt(TextStyle::Outline(0.5,Palette::Black),390,380,ColorF{0.9});
    FontAsset(U"Black")(U"ランキング").drawAt(TextStyle::Outline(0.5,Palette::Black),40,760,560,ColorF{0.9});
    FontAsset(U"Black")(U"あそびかた").drawAt(TextStyle::Outline(0.5,Palette::Black),40,1090,480,ColorF{0.9});

    message_window.draw(ColorF{0.0,0.4});
    
    if(buttons[0].contains(Cursor::Pos())){
        FontAsset(U"Regular")(U"はじめてのかたはこちらから").drawAt(message_window.center(),Palette::White);
    }
    else if(buttons[1].contains(Cursor::Pos())){
        FontAsset(U"Regular")(U"連勝めざしてひたすら一筆書き！").drawAt(message_window.center(),Palette::White);
    }
    else if(buttons[2].contains(Cursor::Pos())){
        FontAsset(U"Regular")(U"ランキングをみる").drawAt(message_window.center(),Palette::White);
    } else if(buttons[3].contains(Cursor::Pos())){
        FontAsset(U"Regular")(U"ルールをよむ").drawAt(message_window.center(),Palette::White);
    } else{
        FontAsset(U"Regular")(U"モードをえらんで長押しで決定").drawAt(message_window.center(),Palette::White);
    }
}
