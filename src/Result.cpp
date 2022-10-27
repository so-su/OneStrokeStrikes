#include "Result.hpp"

Result::Result(const InitData& init) : IScene{init} {}

void Result::update() {
    if (MouseL.down()) {
        // タイトルシーンへ
        changeScene(State::Title);
    }
}

void Result::draw() const {
    Scene::SetBackground(ColorF{0.9});

    const auto& data = getData();

    if (data.win) {
        FontAsset(U"Result")(U"Clear!").drawAt(Scene::Center(), ColorF{0.25});
    }
    else{
        FontAsset(U"Result")(U"Failure...").drawAt(Scene::Center(), ColorF{0.25});
    }
}
