#include "Result.hpp"

Result::Result(const InitData& init) : IScene{init} {

}

void Result::update() {
    if (MouseL.down()) {
        // タイトルシーンへ
        changeScene(State::Title);
    }
}

void Result::draw() const {
    Scene::SetBackground(ColorF{0.4, 0.6, 0.9});

    FontAsset(U"Result")(U"Result").drawAt(400, 60);

    const auto& data = getData();

    if(data.score){
        FontAsset(U"Result")(*data.score).drawAt(Scene::Center(), ColorF{0.25});
    }
}
