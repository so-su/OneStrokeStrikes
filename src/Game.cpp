#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init} {
    getData().score = 100;
}

void Game::update() {
    if (enemy.is_vanishing()) {
        if(drawing_path){
            // パスのスコアによって効果を得る
            auto score=enemy.get_path_score();
            player.get_healed(score.green);
            player.get_ap(score.red);
            player.get_sp(score.blue);
            drawing_path = false;
            if(player.ap_is_full()){
                player.reset_ap();
                alpha_enemy.get_damaged(3);
            }
        }
        if (enemy.vanish()) {
            enemy.initialize();
        }
    }

    if(KeyR.down()){
        enemy.reverse_path();
    }
    
    if(KeyE.down() and player.sp_is_full()){
        player.reset_sp();
        enemy.initialize();
    }
    
    drawing_path ^= MouseL.down();

    if (drawing_path) {
        enemy.update_path(Cursor::Pos());
    } else {
        enemy.clear_path();
    }
    
    if(enemy.update_gauge()){
        player.get_damaged(enemy.attack_value());
    }
    
    int32 full_num=alpha_enemy.update_gauges();
    player.get_damaged(full_num*alpha_enemy.attack_value());
    
    if(not alpha_enemy.alive()){
        changeScene(State::Game);
    }
    
    if(not player.alive()){
        changeScene(State::Game);
    }
    
}

void Game::draw() const {
    Scene::SetBackground(ColorF{0.9});
    
    player.draw();

    enemy.draw();
    if (not enemy.is_vanishing()) {
        enemy.draw_path();
        enemy.draw_gauge();
    }

    alpha_enemy.draw();
    alpha_enemy.draw_gauges();
}
