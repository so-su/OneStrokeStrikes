#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init} {
    getData().score = 100;
}

void Game::update() {
    if (enemy.is_vanishing()) {
        drawing_path = false;
        if (enemy.vanish()) {
            enemy.initialize();
            if(alpha_enemy.alive()){
                player.get_damaged(alpha_enemy.attack_value());
                if(not player.alive()){
                    changeScene(State::Game);
                }
            }
            else{
                alpha_enemy.AlphaEnemy::initialize();
            }
        }
        return;
    }

    if(KeyR.pressed()){
        enemy.reverse_path();
    }
    
    drawing_path ^= MouseL.down();

    if (drawing_path) {
        enemy.update_path(Cursor::Pos());
    } else {
        enemy.clear_path();
    }
}

void Game::draw() const {
    Scene::SetBackground(ColorF{0.9});
    
    player.draw();

    enemy.draw();
    if (not enemy.is_vanishing()) {
        enemy.draw_path();
    }

    alpha_enemy.draw();
}
