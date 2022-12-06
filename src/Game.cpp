#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init},enemies{Enemy(Point{250,500}),Enemy(Point{700,500}),Enemy(Point{1150,500})},drawing_path_idx(none),attack_mode(false),respawn_timers{0.0,0.0,0.0},respawn_time(6.0),all_clear_status{AllClearStatus::EnemyAliveExists},pause(false),attack_shape(nullptr) {
}

void Game::update() {
    mask_alpha_transition.update(attack_mode or pause);

    if(pause){
        if(MouseL.down()){
            pause=false;
        }
        return;
    }
    
    if(attack_shape!=nullptr){
        if(MouseL.down()){
            alpha_enemy.get_damaged(attack_shape);
            
            attack_shape=nullptr;
            attack_mode=false;
            attack_mode_timer.pause();
            roulette.initialize();
            if(alpha_enemy.is_alive()){
                pause=true;
            }
        }
        return;
    }
    
    // アタックモードのときの処理
    if(attack_mode){
        const double time=attack_mode_timer.sF();
        if(time<1.0){
            roulette.go_around(20.0);
        }
        else if(time<roulette_duration){
            double roulette_speed=20.0*(1.0-attack_mode_timer.sF()/roulette_duration);
            roulette.go_around(roulette_speed);
        }
        else if(time>=roulette_duration+1.0){
            AttackType attack_type=roulette.get_attack_type();
            if(attack_type==AttackType::Shape){
                attack_shape=roulette.get_attack_shape();
            }
            else if(attack_type==AttackType::Num){
                alpha_enemy.get_damaged(roulette.get_attack_num());
                
                attack_mode=false;
                attack_mode_timer.pause();
                roulette.initialize();
                if(alpha_enemy.is_alive()){
                    pause=true;
                }
            }
        }
        return;
    }
    
    if(roulette.pressed()){
        pause=true;
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
            vanishing_timers[enemy_idx]=0.0;
            // リスポーン時間は短かめ
            respawn_timers[enemy_idx]=respawn_time-2.0;
            // 有効なセルをシャッフルして消滅の準備をする
            enemy.prepare_to_randomly_vanish();
            // エフェクトを描画
            enemy.add_ring_effect();
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
            
            // パスの端点の色によってルーレットの割合を更新
            roulette.update_value(score.green_bonus, score.red_bonus, score.blue_bonus);
            
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
    
    Triangle{0,400,0,395,350,200}.draw(Palette::Dimgray);
    Triangle{1400,400,1400,395,1050,200}.draw(Palette::Dimgray);
    
    player.draw();
    
    roulette.draw_small_disk();

    for(auto enemy_idx:step(3)){
        const auto& enemy=enemies[enemy_idx];
        if(enemy.has_vanished())continue;
        
        enemy.draw();
        if (not enemy.is_vanishing()) {
            enemy.draw_path();
            enemy.draw_gauge();
        }
        // 消滅が始まってもしばらくはパスを描画する
        // ただし、SPを使って倒した場合は描画しない
        else if(vanishing_timers[enemy_idx]<=0.49 and respawn_timers[enemy_idx]<0.5){
            enemy.draw_path();
        }
    }

    alpha_enemy.draw();
    alpha_enemy.draw_gauges();
    
    {
        const ScopedRenderStates2D blend{ BlendState::Additive };
        for(const auto& enemy:enemies){
            enemy.draw_effect();
        }
    }
    
    if(all_clear_status==AllClearStatus::LastHasVanished){
        FontAsset(U"Kaisotai")(U"All Clear!!").drawAt(100,700,500,Palette::White);
    }
    
    // アタックモード中のマスクを描画
    mask.draw(ColorF{background_color,mask_alpha_transition.value()*0.8});
    
    if(attack_mode or pause){
        roulette.draw();
    }
    
    if(attack_shape!=nullptr){
        Point pos=Cursor::Pos();
        Point upper_left=alpha_enemy.upper_left-Point{3000,3000};
        
        Point center=(pos-upper_left)/30*30+upper_left+Point{15,15};
        attack_shape->draw(center,roulette.chosen_color().withAlpha(static_cast<uint32>(255*(0.3+Periodic::Jump0_1(2s)*0.7))));
    }
}
