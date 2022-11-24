#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init},enemies{Enemy(Point{250,500}),Enemy(Point{700,500}),Enemy(Point{1150,500})},drawing_path_idx(none),attack_mode(false),respawn_timers{0.0,0.0,0.0},respawn_time(6.0),all_clear_status{AllClearStatus::EnemyAliveExists} {
}

void Game::update() {
    mask_alpha_transition.update(attack_mode);
    // アタックモードのときの処理
    if(attack_mode){
        const double time=attack_mode_timer.sF();
        if(time<1.0){
            roulette_pos+=20.0*Scene::DeltaTime();
        }else if(time<roulette_duration){
            double roulette_speed=20.0*(1.0-attack_mode_timer.sF()/roulette_duration);
            roulette_pos+=roulette_speed*Scene::DeltaTime();
        }else if(time>=roulette_duration+1.0){
            const int32 roulette_idx=static_cast<int>(roulette_pos)%4;
            if(roulette_idx==3){
                alpha_enemy.get_damaged(10);
                attack_mode=false;
            }
            else{
                alpha_enemy.get_damaged(5);
                attack_mode=false;
            }
            attack_mode_timer.pause();
        }
        return;
    }
    
    for(auto enemy_idx: step(3)){
        auto& enemy=enemies[enemy_idx];
        if (enemy.is_vanishing() and not enemy.has_vanished()) {
            auto& timer=vanishing_timers[enemy_idx];
            timer+=Scene::DeltaTime();
            if(timer>=0.53){
                // 消滅を進める
                bool has_vanished=enemy.vanish();
                if(all_clear_status==AllClearStatus::LastIsVanishing and has_vanished and enemies[0].has_vanished() and enemies[1].has_vanished() and enemies[2].has_vanished()){
                    all_clear_status=AllClearStatus::LastHasVanished;
                    player.get_healed(300);
                    player.get_ap(300);
                    player.get_sp(300);
                    respawn_timers[0]=respawn_timers[1]=respawn_timers[2]=respawn_time-2.0;
                }
                timer-=0.03;
            }
        }
    }
    
    // SPを消費してスペシャルを使い、敵を一掃する
    if(KeyF.down() and player.sp_is_full()){
        drawing_path_idx=none;
        player.reset_sp();
        for(auto enemy_idx:step(3)){
            auto& enemy=enemies[enemy_idx];
            if(enemy.is_vanishing() or enemy.has_vanished()){
                continue;
            }
            vanishing_timers[enemy_idx]=0.5;
            // リスポーン時間は短かめ
            respawn_timers[enemy_idx]=respawn_time-2.0;
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
                if(enemy_frames[enemy_idx].contains(Cursor::Pos()) and not enemies[enemy_idx].is_vanishing()){
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
            
            bool all_clear = (enemies[0].is_vanishing() or enemies[0].has_vanished())
            and (enemies[1].is_vanishing() or enemies[1].has_vanished())
            and (enemies[2].is_vanishing() or enemies[2].has_vanished());
            if(all_clear){
                all_clear_status=AllClearStatus::LastIsVanishing;
            }
        }
    }
    
    for(auto& enemy:enemies){
        if(enemy.has_vanished())continue;
        if(enemy.update_gauge()){
            player.get_damaged(enemy.attack_value());
        }
    }
    
    // Enemyをスポーンさせる
    for(auto enemy_idx:step(3)){
        auto& timer=respawn_timers[enemy_idx];
        if(enemies[enemy_idx].has_vanished()){
            timer+=Scene::DeltaTime();
        }
        if(timer>=respawn_time and all_clear_status!=AllClearStatus::LastIsVanishing){
            enemies[enemy_idx].initialize();
            timer=0.0;
            all_clear_status=AllClearStatus::EnemyAliveExists;
        }
    }
    
    // AlphaEnemyのゲージを進める
    int32 num_full_gauges=alpha_enemy.update_gauges(false);
    for(auto& enemy:enemies){
        enemy.speed_up_gauge(num_full_gauges);
    }
    
    // APを消費してアタックモードに切り替わる
    if(KeyD.down() and player.ap_is_full()){
        attack_mode=true;
        attack_mode_timer.restart();
        
        roulette_pos=0.0;
        roulette_duration=Random(2.5,4.0);
        
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
    Scene::SetBackground(background_color);
    
    player.draw();

    for(auto enemy_idx:step(3)){
        const auto& enemy=enemies[enemy_idx];
        if(enemy.has_vanished())continue;
        
        enemy.draw();
        if (not enemy.is_vanishing()) {
            enemy.draw_path();
            enemy.draw_gauge();
        }
        // 消滅が始まってもしばらくはパスを描画する
        else if(vanishing_timers[enemy_idx]<=0.49){
            enemy.draw_path();
        }
    }

    alpha_enemy.draw();
    alpha_enemy.draw_gauges();
    
    attack_shapes.draw();
    
    {
        const ScopedRenderStates2D blend{ BlendState::Additive };
        for(const auto& enemy:enemies){
            enemy.draw_effect();
        }
    }
    
    if(all_clear_status==AllClearStatus::LastHasVanished){
        FontAsset(U"Result")(U"All Clear!!!").drawAt(Scene::Center(), ColorF{0.9});
    }
    
    // アタックモード中のマスクを描画
    mask.draw(ColorF{background_color,mask_alpha_transition.value()*0.5});
    
    if(attack_mode){
        int32 roulette_idx=static_cast<int>(roulette_pos)%4;
        Triangle{ static_cast<double>(1130+70*roulette_idx), 100, 10, 180_deg }.draw(Palette::White);
    }
}
