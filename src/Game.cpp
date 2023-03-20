#include "Game.hpp"

Game::Game(const InitData& init)
    : IScene{init}
    {}

void Game::update() {
    // マスクの透過率を更新
    mask_alpha_transition.update(attack_mode or pause);

    // ポーズ中
    if (pause) {
        if (MouseL.down()) {
            pause = false;
        }
        return;
    }

    // 図形で攻撃する位置を選んでいるとき
    if (attack_shape != nullptr) {
        shape_attack_update();
        return;
    }

    // アタックモードのときの処理
    if (attack_mode) {
        attack_mode_update();
        return;
    }

    // 小さなルーレットが押されたらポーズ画面に遷移
    if (roulette.down()) {
        pause = true;
        return;
    }

    // Enemyたちの消滅を進める
    update_to_vanish_enemies();

    // SPが満タンの状態で[F]キーが押されたら、SPを消費して敵を一掃する
    if (KeyF.down() and player.sp_is_full()) {
        use_sp();
    }

    // 画面をクリックしたとき
    if (MouseL.down()) {
        // 既に一筆書き中なら、一筆書き状態を解除する
        if (drawing_path_idx.has_value()) {
            enemies[*drawing_path_idx].clear_path();
            drawing_path_idx = none;
        }
        // 一筆書き中でないなら、一筆書き状態へ遷移
        else {
            for (auto enemy_idx : step(3)) {
                if (enemy_frames[enemy_idx].contains(Cursor::Pos()) and
                    not enemies[enemy_idx].is_vanishing()) {
                    drawing_path_idx = enemy_idx;
                    break;
                }
            }
        }
    }

    // [Space]キーが押されたら一筆書きのパスを反転させる
    if (KeySpace.down() and drawing_path_idx.has_value()) {
        enemies[*drawing_path_idx].reverse_path();
    }

    // 一筆書き中のとき
    if (drawing_path_idx.has_value()) {
        update_one_stroke_path();
    }

    // Enemyのゲージを進めて、一周したゲージの数だけダメージを受ける
    for (auto& enemy : enemies) {
        if (not enemy.is_alive()) continue;
        if (enemy.update_gauge()) {
            player.get_damaged(enemy.attack_value());
        }
    }

    // Enemyたちをリスポーンさせる
    for (auto enemy_idx : step(3)) {
        auto& timer{respawn_timers[enemy_idx]};
        if (not enemies[enemy_idx].is_alive()) {
            timer += Scene::DeltaTime();
        }
        
        // All Clearの効果を待機している状態LastIsVanishingのときはリスポーンさせない
        if (timer > Parameter::respawn_time and
            all_clear_status != AllClearStatus::LastIsVanishing) {
            enemies[enemy_idx].initialize();
            timer = 0.0;
            all_clear_status = AllClearStatus::EnemyAliveExists;
        }
    }

    // AlphaEnemyのゲージを進めて、一周したゲージの数だけEnemyたちのゲージを加速する
    int32 num_full_gauges{alpha_enemy.update_gauges()};
    for (auto& enemy : enemies) {
        enemy.speed_up_gauge(num_full_gauges);
    }

    // APを消費してアタックモードに切り替わる
    if (KeyD.down() and player.ap_is_full()) {
        use_ap();
    }

    // プレイヤーの勝利
    if (not alpha_enemy.is_alive()) {
        ++getData().win;
        
        if (getData().easy_mode) {
            changeScene(State::Result);
        }
        // 次のゲームへ遷移
        else {
            changeScene(State::Game);
        }
    }

    // プレイヤーの敗北
    if (not player.is_alive()) {
        changeScene(State::Result);
    }
}

void Game::draw() const {
    Scene::SetBackground(background_color);

    // 奥行きを見せるための背景の線
    //Triangle{0, 400, 0, 395, 350, 200}.draw(Palette::Dimgray);
    //Triangle{1400, 400, 1400, 395, 1050, 200}.draw(Palette::Dimgray);

    // プレイヤーのステータスを描画
    player.draw();

    // 小さいルーレットを画面右下に描画
    roulette.draw_small_disk();

    // Enemyたちの描画
    for (auto enemy_idx : step(3)) {
        const auto& enemy{enemies[enemy_idx]};
        if (not enemy.is_alive()) continue;

        enemy.draw();
        
        // 消滅中は描画しない
        if (not enemy.is_vanishing()) {
            enemy.draw_path();
            enemy.draw_gauge();
        }
        // 消滅が始まってもしばらくはパスを描画する
        // ただし、SPを使って倒した場合は描画しない
        else if (vanishing_timers[enemy_idx] < Parameter::time_until_vanishing and
                 respawn_timers[enemy_idx] < Parameter::respawn_time - Parameter::short_respawn_time - 1e-5) {
            enemy.draw_path();
        }
    }

    // AlphaEnemyの描画
    alpha_enemy.draw();
    alpha_enemy.draw_gauges();

    {
        // エフェクトは加算ブレンドで描画する
        const ScopedRenderStates2D blend{BlendState::Additive};
        for (const auto& enemy : enemies) {
            enemy.draw_effect();
        }
    }

    if (all_clear_status == AllClearStatus::LastHasVanished) {
        FontAsset(U"Kaisotai")(U"All Clear!!")
            .drawAt(100, 700, 500, Palette::White);
    }

    // アタックモード中のマスクを描画
    mask.draw(ColorF{background_color, mask_alpha_transition.value() * 0.8});

    if (attack_mode or pause) {
        // 中央に大きなルーレットを描画する
        roulette.draw();
    }

    // 図形で攻撃する位置を選んでいるとき
    if (attack_shape != nullptr) {
        const Point upper_left = alpha_enemy.upper_left - Point{3000, 3000};
        const Point center =
            (Cursor::Pos() - upper_left) / 30 * 30 + upper_left + Point{15, 15};
        
        // 透過率を調整して、点滅するようAttackShapeを描画する
        attack_shape->draw(
            center, roulette.chosen_color().withAlpha(static_cast<uint32>(
                        255 * (0.3 + Periodic::Jump0_1(2s) * 0.7))));
    }
}


// 図形で攻撃する位置を選んでいるときの処理
void Game::shape_attack_update(){
    if (not MouseL.down()) return;
    
    // 図形で攻撃し、ひとつも消えなかったらやり直し
    if(not alpha_enemy.get_damaged(attack_shape)){
        return;
    }
    
    attack_shape = nullptr;
    get_out_of_attack_mode();
}

// 攻撃モード時の処理
void Game::attack_mode_update(){
    const double time{attack_mode_timer.sF()};
    
    // 等速で回転する期間
    if (time < 1.0) {
        roulette.go_around(Parameter::roulette_rotation_max_speed);
    }
    // 回転速度が線形に減少していく期間
    else if (time < roulette_duration) {
        const double roulette_speed{Parameter::roulette_rotation_max_speed * (1.0 - attack_mode_timer.sF() / roulette_duration)};
        roulette.go_around(roulette_speed);
    }
    // ルーレットが止まってから1.0秒待機し、攻撃方法を決定
    else if (time > roulette_duration + 1.0) {
        const AttackType attack_type{roulette.get_attack_type()};
        if (attack_type == AttackType::Shape) {
            attack_shape = roulette.get_attack_shape();
        } else if (attack_type == AttackType::Num) {
            alpha_enemy.get_damaged(roulette.get_attack_num());
            get_out_of_attack_mode();
        }
    }
}

// 攻撃モードから抜ける処理
void Game::get_out_of_attack_mode(){
    attack_mode = false;
    attack_mode_timer.pause();
    roulette.initialize();
    
    // 次の攻撃時のルーレットを表示するためにポーズ画面へ遷移
    if (alpha_enemy.is_alive()) {
        pause = true;
    }
}

// Enemyたちの消滅を進める
void Game::update_to_vanish_enemies(){
    for (auto enemy_idx : step(3)) {
        auto& enemy{enemies[enemy_idx]};
        
        if (not (enemy.is_vanishing() and enemy.is_alive())){
            continue;
        }
        
        auto& timer{vanishing_timers[enemy_idx]};
        timer += Scene::DeltaTime();
        if (timer > Parameter::time_until_vanishing) {
            // 消滅を進める
            bool has_vanished{enemy.vanish()};
            
            // All Clearの状態に遷移するとき
            if (all_clear_status == AllClearStatus::LastIsVanishing and
                has_vanished and (not enemies[0].is_alive()) and
                (not enemies[1].is_alive()) and (not enemies[2].is_alive())) {
                all_clear_status = AllClearStatus::LastHasVanished;
                
                // All Clearボーナスを得る
                player.get_healed(Parameter::all_clear_bonus);
                player.get_ap(Parameter::all_clear_bonus);
                player.get_sp(Parameter::all_clear_bonus);
                
                // リスポーンまでの時間は短め
                respawn_timers[0] = respawn_timers[1] = respawn_timers[2] =
                    Parameter::respawn_time - Parameter::short_respawn_time;
            }
        }
    }
}

// SPを消費して敵を一掃する
void Game::use_sp(){
    drawing_path_idx = none;
    player.reset_sp();
    
    for (auto enemy_idx : step(3)) {
        auto& enemy{enemies[enemy_idx]};
        
        // 既に倒されているものはスルー
        if (enemy.is_vanishing() or not enemy.is_alive()) {
            continue;
        }
        
        // リスポーンまでの時間は短め
        respawn_timers[enemy_idx] = Parameter::respawn_time - Parameter::short_respawn_time;
        
        vanishing_timers[enemy_idx] = 0.0;
        enemy.prepare_to_randomly_vanish();
        enemy.add_ring_effect();
        
        // SPで倒したときには基礎スコアだけを得る
        const auto& score{enemy.get_basic_score()};
        player.get_healed(score.green);
        player.get_ap(score.red);
        player.get_sp(score.blue);
    }
}

// 一筆書きのパスを更新し、一筆書きが成功したならEnemyを消滅させる準備をする
void Game::update_one_stroke_path(){
    auto& enemy = enemies[*drawing_path_idx];
    
    // パスを更新
    enemy.update_path(Cursor::Pos());
    
    // 一筆書きが成功したときの処理
    if (enemy.is_vanishing()) {
        vanishing_timers[*drawing_path_idx] = 0.0;
        drawing_path_idx = none;

        // パスのスコアによって効果を得る
        const auto& score{enemy.get_path_score()};
        player.get_healed(score.green);
        player.get_ap(score.red);
        player.get_sp(score.blue);

        // パスの端点の色によってルーレットの割合を更新
        roulette.update_value(score.green_endpoint, score.red_endpoint,
                              score.blue_endpoint);
        
        // All Clearの状態か
        bool all_clear =
            (enemies[0].is_vanishing() or not enemies[0].is_alive()) and
            (enemies[1].is_vanishing() or not enemies[1].is_alive()) and
            (enemies[2].is_vanishing() or not enemies[2].is_alive());
        
        // すべてのEnemyが消滅するまで待ってからAll Clearの効果を得る
        // そのため、いまはLastIsVanishingの状態にしておく
        if (all_clear) {
            all_clear_status = AllClearStatus::LastIsVanishing;
        }
    }
}

// APを消費して攻撃モードに遷移する
void Game::use_ap(){
    attack_mode = true;
    attack_mode_timer.restart();
    player.reset_ap();
    
    // 敵のゲージもリセット
    for (auto& enemy : enemies) {
        enemy.reset_gauge();
    }
    alpha_enemy.reset_gauge();

    // ルーレットが回る時間をランダムで決める
    roulette_duration = Random(Parameter::roulette_rotation_min_duration, Parameter::roulette_rotation_max_duration);
}
