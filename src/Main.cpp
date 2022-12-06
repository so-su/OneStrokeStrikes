#include "Common.hpp"
#include "Game.hpp"
#include "Result.hpp"
#include "Title.hpp"

void Main() {
    Window::Resize(1400, 800);
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);

    FontAsset::Register(U"Kaisotai", FontMethod::MSDF, 150, U"example/font/kaiso_up/Kaisotai-Next-UP-B.ttf");
    FontAsset::Register(U"Menu", FontMethod::MSDF, 40, Typeface::Medium);
    FontAsset::Register(U"Result", 40, Typeface::Heavy);
    FontAsset::Register(U"GameScore", 30, Typeface::Light);

    App manager;
    manager.add<Title>(State::Title);
    manager.add<Game>(State::Game);
    manager.add<Result>(State::Result);

    // ゲームシーンから開始したい場合はこのコメントを外す
    //manager.init(State::Game);

    while (System::Update()) {
        if (not manager.update()) {
            break;
        }
    }
}
