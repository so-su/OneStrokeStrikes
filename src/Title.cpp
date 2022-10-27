#include "Title.hpp"

Title::Title(const InitData& init) : IScene{init},m_startButton{Arg::center = Scene::Center(), 300, 60} {}

void Title::update() {
    if (m_startButton.mouseOver()) {
        Cursor::RequestStyle(CursorStyle::Hand);
    }

    if (m_startButton.leftClicked()) {
        // ゲームシーンへ
        changeScene(State::Game);
    }
}

void Title::draw() const {
    Scene::SetBackground(ColorF{0.9});

    FontAsset(U"TitleFont")(U"One Stroke Strikes")
        .drawAt(70, Vec2{700, 150},Palette::Black);

    m_startButton.draw(ColorF{0.9}).drawFrame(2,Palette::Black);

    FontAsset(U"Menu")(U"PLAY").drawAt(m_startButton.center(), ColorF{0.25});
}
