#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init} { getData().score = 100; }

void Game::update() {}

void Game::draw() const {
    Scene::SetBackground(ColorF{0.9});
    enemy.draw();
    alpha_enemy.draw();
}
