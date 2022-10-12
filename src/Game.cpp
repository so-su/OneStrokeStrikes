#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init} {

}

void Game::update() {
    getData().score = 100;
    
    if (MouseL.down()) {
        // タイトルシーンへ
        changeScene(State::Result);
    }
}

void Game::draw() const {
    Scene::SetBackground(ColorF{0.2});
}

