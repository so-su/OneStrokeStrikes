#include "Result.hpp"

Result::Result(const InitData& init) : IScene{init} {
    getData().score=100*getData().win*(getData().win+1)/2+getData().enemy;
}

void Result::update() {
    if (MouseL.down()) {
        // タイトルシーンへ
        changeScene(State::Game);
    }
}

void Result::draw() const {
    Scene::SetBackground(ColorF{0.9});
    
    FontAsset(U"Kaisotai")(U"リザルト").drawAt(80,700,100,Palette::Black);
    frame.drawFrame(1,1,Palette::Dimgray);
    
    if(getData().easy_mode){
        if(getData().win>0){
            FontAsset(U"Black")(U"勝ち！").drawAt(80,Scene::Center(), Palette::Black);
        }
        else{
            FontAsset(U"Regular")(U"負けてしまった...").drawAt(80,Scene::Center(), Palette::Black);
        }
    }
    else{
        if (getData().win>0) {
            FontAsset(U"Kaisotai")(U"WIN!").drawAt(100,Scene::Center(), Palette::Black);
        }
        else{
            FontAsset(U"Kaisotai")(U"LOSE...").drawAt(100,Scene::Center(), Palette::Black);
        }
        
        if(getData().win>=2){
            FontAsset(U"GameScore")(U"{}連勝中"_fmt(getData().win)).drawAt(Scene::Center().x,Scene::Center().y+50, Palette::Black);
        }
            
    }
}
