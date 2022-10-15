#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init} {
    enemy = new Enemy();
    getData().score = 100;
}

void Game::update() {
    if (enemy->is_vanishing()) {
        drawing_path = false;
        if (enemy->vanish()) {
            delete enemy;
            enemy = new Enemy();
        }
        return;
    }

    drawing_path ^= MouseL.down();

    if (drawing_path) {
        enemy->update_path(Cursor::Pos());
    } else {
        enemy->clear_path();
    }
}

void Game::draw() const {
    Scene::SetBackground(ColorF{0.9});

    enemy->draw();
    if (not enemy->is_vanishing()) {
        enemy->draw_path();
    }

    alpha_enemy.draw();
}
