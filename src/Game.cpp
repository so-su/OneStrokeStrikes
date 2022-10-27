#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init},enemies{Enemy(Point{250,500}),Enemy(Point{700,500}),Enemy(Point{1150,500})},drawing_path_idx(none),attack_mode(false) {
}

void Game::update() {
    mask_alpha_transition.update(attack_mode);
    // アタックモードのときの処理
    if(attack_mode){
        // アタックモードに入ってからターゲットせずに10秒経過したらターゲットせずに攻撃
        if(attack_mode_timer.sF()>=10.0){
            alpha_enemy.get_damaged(5);
            attack_mode=false;
        }
        else if(MouseL.down()){
            alpha_enemy.get_damaged(5,Cursor::Pos());
            attack_mode=false;
        }
        
        // マウスオーバーで連結成分の色を変化させる
        alpha_enemy.mouse_over(attack_mode?Cursor::Pos():Point{-1,-1});
        
        return;
    }
    
    for(auto enemy_idx: step(3)){
        auto& enemy=enemies[enemy_idx];
        if (enemy.is_vanishing() and not enemy.has_vanished()) {
            if (enemy.vanish()) {
                enemy_idx_queue.push(enemy_idx);
            }
        }
    }
    
    // SPを消費してスペシャルを使い、敵を一掃する
    if(KeyF.down() and player.sp_is_full() and size(enemy_idx_queue)<3){
        drawing_path_idx=none;
        player.reset_sp();
        for(auto& enemy : enemies){
            // 有効なセルをシャッフルして消滅の準備をする
            enemy.prepare_to_randomly_vanish();
            // スコアを得る
            auto score=enemy.get_ordinary_score();
            player.get_healed(score.green);
            player.get_ap(score.red);
            player.get_sp(score.blue);
        }
        drawing_path_idx=none;
    }
    
    if(MouseL.down()){
        if(drawing_path_idx.has_value()){
            enemies[*drawing_path_idx].clear_path();
            drawing_path_idx=none;
        }
        else{
            for(auto enemy_idx:step(3)){
                if(enemy_frames[enemy_idx].contains(Cursor::Pos())){
                    drawing_path_idx=enemy_idx;
                    break;
                }
            }
        }
    }
    
    // パスを反転させる
    if(KeySpace.down() and drawing_path_idx.has_value()){
        enemies[*drawing_path_idx].reverse_path();
    }

    if (drawing_path_idx.has_value()) {
        auto& enemy=enemies[*drawing_path_idx];
        enemy.update_path(Cursor::Pos());
        if(enemy.is_vanishing()){
            drawing_path_idx=none;
            
            // パスのスコアによって効果を得る
            auto score=enemy.get_path_score();
            player.get_healed(score.green);
            player.get_ap(score.red);
            player.get_sp(score.blue);
        }
    }
    
    for(auto& enemy:enemies){
        if(enemy.has_vanished())continue;
        if(enemy.update_gauge()){
            player.get_damaged(enemy.attack_value());
        }
    }
    stop_watch.start();
    bool speed_up=size(enemy_idx_queue)==3;
    if(speed_up){
        if(not stop_watch.isRunning()){
            stop_watch.start();
        }
        if(stop_watch.msF()>=10.0){
            player.get_ap(1);
            player.get_sp(1);
            stop_watch.restart();
        }
    }
    else if(stop_watch.isRunning()){
        stop_watch.reset();
    }
    
    int32 full_num=alpha_enemy.update_gauges(speed_up);
    while(not enemy_idx_queue.empty() and full_num>0){
        int32 enemy_idx=enemy_idx_queue.front();
        enemies[enemy_idx].initialize();
        enemy_idx_queue.pop();
        --full_num;
    }
    player.get_damaged(full_num*alpha_enemy.attack_value());
    
    // APを消費してアタックモードに切り替わる
    if(KeyD.down() and player.ap_is_full()){
        attack_mode=true;
        attack_mode_timer.restart();
        player.reset_ap();
        
        for(auto& enemy:enemies){
            enemy.reset_gauge();
        }
        alpha_enemy.reset_gauge();
    }
    
    if(not alpha_enemy.alive()){
        getData().win=true;
        changeScene(State::Result);
    }
    
    if(not player.alive()){
        getData().win=false;
        changeScene(State::Result);
    }
}

void Game::draw() const {
    Scene::SetBackground(ColorF{0.9});
    
    player.draw();

    for(const auto& enemy:enemies){
        if(enemy.has_vanished())continue;
        enemy.draw();
        if (not enemy.is_vanishing()) {
            enemy.draw_path();
            enemy.draw_gauge();
        }
    }

    alpha_enemy.draw();
    alpha_enemy.draw_gauges();
    
    mask.draw(ColorF{0.0,0.0,0.0,mask_alpha_transition.value()*0.5});
}
