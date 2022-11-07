#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init},enemies{Enemy(Point{250,500}),Enemy(Point{700,500}),Enemy(Point{1150,500})},drawing_path_idx(none),attack_mode(false),speed_up(false) {
}

void Game::update() {
    mask_alpha_transition.update(attack_mode);
    // アタックモードのときの処理
    if(attack_mode){
        // アタックモードに入ってからターゲットせずに10秒経過したらターゲットせずに攻撃
        if(attack_mode_timer.sF()>=30.0){
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
            auto& timer=vanishing_timers[enemy_idx];
            timer+=Scene::DeltaTime();
            if(timer>=0.53){
                // 消滅を進める
                bool has_vanished = enemy.vanish();
                if (has_vanished) {
                    enemy_idx_queue.push(enemy_idx);
                }
                timer-=0.03;
            }
        }
    }
    
    // SPを消費してスペシャルを使い、敵を一掃する
    if(KeyF.down() and player.sp_is_full() and size(enemy_idx_queue)<3){
        drawing_path_idx=none;
        player.reset_sp();
        for(auto enemy_idx:step(3)){
            auto& enemy=enemies[enemy_idx];
            vanishing_timers[enemy_idx]=0.0;
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
            vanishing_timers[*drawing_path_idx]=0.0;
            
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
    
    speed_up=size(enemy_idx_queue)==3;
    if(speed_up){
        if(not speed_up_stop_watch.isRunning()){
            speed_up_stop_watch.start();
        }
        if(speed_up_stop_watch.msF()>=50.0){
            player.get_ap(10);
            player.get_sp(10);
            speed_up_stop_watch.set(speed_up_stop_watch.elapsed()-50ms);
        }
        
        if(not speed_up_stop_watch_2nd.isRunning()){
            speed_up_stop_watch_2nd.start();
        }
        if(speed_up_stop_watch_2nd.msF()>=800.0){
            speed_up_stop_watch_2nd.set(speed_up_stop_watch_2nd.elapsed()-800ms);
        }
    }
    else{
        if(speed_up_stop_watch.isRunning()){
            speed_up_stop_watch.reset();
        }
        if(speed_up_stop_watch_2nd.isRunning()){
            speed_up_stop_watch_2nd.reset();
        }
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
    
    // プレイヤーの勝ち
    if(not alpha_enemy.is_alive()){
        getData().win=true;
        ++getData().win_cnt;
        changeScene(State::Result);
    }
    
    // プレイヤーの負け
    if(not player.is_alive()){
        getData().win=false;
        getData().win_cnt=0;
        changeScene(State::Result);
    }
}

void Game::draw() const {
    Scene::SetBackground(ColorF{0.9});
    
    player.draw();

    for(const auto& enemy:enemies){
        if(not enemy.has_vanished()){
            enemy.draw();
            if (not enemy.is_vanishing()) {
                enemy.draw_path();
                enemy.draw_gauge();
            }
        }
        enemy.draw_effect();
    }

    alpha_enemy.draw();
    alpha_enemy.draw_gauges();
    
    // 早送り中ならマークを描画
    if(speed_up and not attack_mode){
        const double t=speed_up_stop_watch_2nd.msF();
        
        if(t>=600.0){
            speed_up_triangle_left.draw(Palette::Black);
            speed_up_triangle_center.draw(Palette::Black);
            speed_up_triangle_right.draw(ColorF{0.0,0.0,0.0,(t-600.0)/150.0});
        }
        else if(t>=400.0){
            speed_up_triangle_left.draw(Palette::Black);
            speed_up_triangle_center.draw(ColorF{0.0,0.0,0.0,(t-400.0)/150.0});
        }
        else if(t>=200.0){
            speed_up_triangle_left.draw(ColorF{0.0,0.0,0.0,(t-200.0)/150.0});
        }
    }
    
    // アタックモード中のマスクを描画
    mask.draw(ColorF{0.0,0.0,0.0,mask_alpha_transition.value()*0.5});
}
