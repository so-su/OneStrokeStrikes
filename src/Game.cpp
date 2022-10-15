#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init} { getData().score = 100; }

void Game::update() {
    drawing_path^=MouseL.down();
    
    if(drawing_path){
        enemy.update_path(Cursor::Pos());
    }else{
        enemy.clear_path();
    }
}

void Game::draw() const {
    Scene::SetBackground(ColorF{0.9});
    enemy.draw();
    enemy.draw_path();
    alpha_enemy.draw();
}
