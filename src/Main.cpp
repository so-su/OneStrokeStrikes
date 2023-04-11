#include "Common.hpp"
#include "Game.hpp"
#include "Ranking.hpp"
#include "Result.hpp"
#include "Title.hpp"

void Main() {
    Window::Resize(1400, 800);
    Window::SetTitle(U"すとすと");

    // ESCキーで終了しないように設定
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);

    // フォントの登録
    FontAsset::Register(U"Kaisotai", FontMethod::MSDF, 150,
                        U"font/kaiso_up/Kaisotai-Next-UP-B.ttf");
    FontAsset::Register(U"Black", FontMethod::MSDF, 80, Typeface::Black);
    FontAsset::Register(U"Regular", FontMethod::MSDF, 30, Typeface::Regular);

    // シーンの登録
    App manager;
    manager.add<Title>(State::Title);
    manager.add<Game>(State::Game);
    manager.add<Result>(State::Result);
    manager.add<Ranking>(State::Ranking);

    while (System::Update()) {
        if (not manager.update()) {
            break;
        }
    }
}
