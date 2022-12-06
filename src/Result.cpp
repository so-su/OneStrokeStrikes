#include "Result.hpp"

Result::Result(const InitData& init) : IScene{init} {}

void Result::update() {
    if (MouseL.down()) {
        // タイトルシーンへ
        changeScene(State::Game);
    }
}

void Result::draw() const {
    Scene::SetBackground(Color{0,0,50});

    const auto& data = getData();

    if (data.win) {
        FontAsset(U"Kaisotai")(U"WIN!").drawAt(100,Scene::Center(), ColorF{0.9});
    }
    else{
        FontAsset(U"Kaisotai")(U"LOSE...").drawAt(100,Scene::Center(), ColorF{0.9});
    }
    
    if(getData().win_cnt>=2){
        FontAsset(U"GameScore")(U"{}連勝中"_fmt(getData().win_cnt)).drawAt(Scene::Center().x,Scene::Center().y+50, ColorF{0.9});
    }
}
