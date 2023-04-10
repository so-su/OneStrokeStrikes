#include "Game.hpp"

Game::Game(const InitData& init) : IScene{init} {
    getData().score = 0;
    getData().win = false;
    getData().elapsed_time = 0.0;
    getData().point_sum = 0;
    getData().attack_combo = 0;
    getData().sum_max_run_length = 0;
    getData().cnt_one_stroke = 0;
}

void Game::update() {
    // [esc]キーでタイトルに戻る
    if(KeyEscape.down()){
        changeScene(State::Title);
    }
    
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
    
    // これより上の処理でreturnしたら時間経過に含めない
    getData().elapsed_time += Scene::DeltaTime();

    // Enemyたちの消滅を進める
    update_to_vanish_enemies();

    // 画面をクリックしたとき
    if (MouseL.down()) {
        // APを消費してアタックモードに切り替わる
        if (Rect{940, 720, 120, 80}.contains(Cursor::Pos()) and player.ap_is_full()) {
            use_ap();
            return;
        }
        // SPを消費して敵を一掃する
        else if (Rect{1090, 720, 120, 80}.contains(Cursor::Pos()) and player.sp_is_full() and all_clear_status == AllClearStatus::EnemyAliveExists) {
            use_sp();
        }
        // 既に一筆書き中なら、一筆書き状態を解除する
        else if (drawing_path_idx.has_value()) {
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

    // [space]キーが押されたら一筆書きのパスを反転させる
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

    // プレイヤーの勝利
    if (not alpha_enemy.is_alive()) {
        getData().win = true;
        changeScene(State::Result);
    }

    // プレイヤーの敗北
    if (not player.is_alive()) {
        getData().win = false;
        changeScene(State::Result);
    }
}

void Game::draw() const {
    /* 描画の順番には気をつける */
    
    Scene::SetBackground(MyColor::Background);
    
    // 操作説明
    FontAsset(U"Black")(U"[space] 一筆書きの始点終点いれかえ").draw(18, 10, 10, Palette::White);
    FontAsset(U"Black")(U"[esc] タイトルへもどる").draw(18, 10, 30, Palette::White);

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
        else if (vanishing_timers[enemy_idx] <
                     Parameter::time_until_vanishing and
                 respawn_timers[enemy_idx] < Parameter::respawn_time -
                                                 Parameter::short_respawn_time -
                                                 1e-5) {
            enemy.draw_path();
        }
    }

    // AlphaEnemyの描画
    alpha_enemy.draw();
    alpha_enemy.draw_gauges();
    
    // プレイヤーのステータスを描画
    player.draw();
    
    // APバーとSPバーのぼかし処理
    blur_bars();
    
    // 小さいルーレットを画面右下に描画
    roulette.draw_small_disk();

    {
        // エフェクトは加算ブレンドで描画する
        const ScopedRenderStates2D blend{BlendState::Additive};
        for (const auto& enemy : enemies) {
            enemy.draw_effect();
        }
    }
    
    // アタックモード中のマスクを描画
    mask.draw(ColorF{MyColor::Background, mask_alpha_transition.value() * 0.8});
    
    // 文字のエフェクトの更新
    effect.update();
   
    if (attack_mode or pause) {
        // 中央に大きなルーレットを描画する
        roulette.draw();
    }
    
    // ルーレットの上下に表示するメッセージ
    if(pause){
        FontAsset(U"Black")(U"つぎ")
            .drawAt(40, 700, 300, Palette::White);
        FontAsset(U"Black")(U"クリックで再開")
            .drawAt(25, 700, 670, Palette::White);
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
void Game::shape_attack_update() {
    if (not MouseL.down()) return;
    
    ShapeAttackStatus status{alpha_enemy.get_damaged(attack_shape)};

    // 図形で攻撃し、ひとつも消えなかったらやり直し
    if (status == ShapeAttackStatus::None) {
        return;
    }

    attack_shape = nullptr;
    get_out_of_attack_mode();
    
    // attack shapeのブロックすべてで削除できたらもう一度（ぴったりコンボ）
    if(status == ShapeAttackStatus::All){
        pause = false;
        attack_mode = true;
        attack_mode_timer.restart();

        // ルーレットが回る時間をランダムで決める
        roulette_duration = Random(Parameter::roulette_rotation_min_duration,
                                   Parameter::roulette_rotation_max_duration);
        
        ++getData().attack_combo;
        
        effect.add<StringEffect>(U"ぴったりコンボ！", 80, Point{700, 320}, 2.0, 0.5);
    }
}

// 攻撃モード時の処理
void Game::attack_mode_update() {
    const double time{attack_mode_timer.sF()};

    // 等速で回転する期間
    if (time < 1.0) {
        roulette.go_around(Parameter::roulette_rotation_max_speed);
    }
    // 回転速度が線形に減少していく期間
    else if (time < roulette_duration) {
        const double roulette_speed{
            Parameter::roulette_rotation_max_speed *
            (1.0 - attack_mode_timer.sF() / roulette_duration)};
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
void Game::get_out_of_attack_mode() {
    attack_mode = false;
    attack_mode_timer.pause();
    roulette.initialize();

    // 次の攻撃時のルーレットを表示するためにポーズ画面へ遷移
    if (alpha_enemy.is_alive()) {
        pause = true;
    }
}

// Enemyたちの消滅を進める
void Game::update_to_vanish_enemies() {
    for (auto enemy_idx : step(3)) {
        auto& enemy{enemies[enemy_idx]};

        if (not(enemy.is_vanishing() and enemy.is_alive())) {
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
                
                effect.add<StringEffect>(U"オールクリア!", 100, Point{700, 500}, 2.0, 0.5);
            }
        }
    }
}

// SPを消費して敵を一掃する
void Game::use_sp() {
    drawing_path_idx = none;
    player.reset_sp();

    for (auto enemy_idx : step(3)) {
        auto& enemy{enemies[enemy_idx]};

        // 既に倒されているものはスルー
        if (enemy.is_vanishing() or not enemy.is_alive()) {
            continue;
        }

        // リスポーンまでの時間は短め
        respawn_timers[enemy_idx] =
            Parameter::respawn_time - Parameter::short_respawn_time;

        vanishing_timers[enemy_idx] = 0.0;
        enemy.prepare_to_randomly_vanish();
        enemy.add_ring_effect();

        // SPで倒したときには基礎スコアだけを得る
        const auto& score{enemy.get_basic_score()};
        player.get_healed(score.green);
        player.get_ap(score.red);
        player.get_sp(score.blue);
        getData().point_sum += score.green + score.red + score.blue;
    }
}

// 一筆書きのパスを更新し、一筆書きが成功したならEnemyを消滅させる準備をする
void Game::update_one_stroke_path() {
    auto& enemy = enemies[*drawing_path_idx];

    // パスを更新
    enemy.update_path(Cursor::Pos());

    // 一筆書きが成功したときの処理
    if (enemy.is_vanishing()) {
        vanishing_timers[*drawing_path_idx] = 0.0;
        drawing_path_idx = none;

        const auto& path_info{enemy.get_path_score()};
        
        getData().sum_max_run_length += path_info.second;
        ++getData().cnt_one_stroke;
        
        // パスのスコアによって効果を得る
        const auto& score{path_info.first};
        player.get_healed(score.green);
        player.get_ap(score.red);
        player.get_sp(score.blue);
        getData().point_sum += score.green + score.red + score.blue;

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
void Game::use_ap() {
    attack_mode = true;
    attack_mode_timer.restart();
    player.reset_ap();

    // 敵のゲージもリセット
    for (auto& enemy : enemies) {
        enemy.reset_gauge();
    }
    alpha_enemy.reset_gauge();

    // ルーレットが回る時間をランダムで決める
    roulette_duration = Random(Parameter::roulette_rotation_min_duration,
                               Parameter::roulette_rotation_max_duration);
}

// APバーとSPバーにぼかし処理を施す
void Game::blur_bars() const {
    // APが満タンのときにぼかし処理
    if(player.ap_is_full()){
        {
            // ガウスぼかし用テクスチャにもう一度バーを描く
            {
                const ScopedRenderTarget2D target{ gaussianA1_ap.clear(ColorF{ 0.0 }) };
                const ScopedRenderStates2D blend{ BlendState::Additive };
                
                player.draw_ap_bar();
            }

            // オリジナルサイズのガウスぼかし (A1)
            // A1 を 1/16 サイズにしてガウスぼかし (A16)
            Shader::Downsample(gaussianA1_ap, gaussianA16_ap);
            Shader::GaussianBlur(gaussianA16_ap, gaussianB16_ap, gaussianA16_ap);
        }

        {
            const ScopedRenderStates2D blend{ BlendState::Additive };

            // マウスオーバーしているならぼかし最大
            gaussianA16_ap.resized(sceneSize).draw(ColorF{ (Rect{940, 720, 120, 80}.contains(Cursor::Pos())? 1.0: Periodic::Jump0_1(2s)) * 3 });
        }
    }
    
    // SPが満タンのときにぼかし処理
    if(player.sp_is_full()){
        {
            // ガウスぼかし用テクスチャにもう一度バーを描く
            {
                const ScopedRenderTarget2D target{ gaussianA1_sp.clear(ColorF{ 0.0 }) };
                const ScopedRenderStates2D blend{ BlendState::Additive };
                
                player.draw_sp_bar();
            }

            // オリジナルサイズのガウスぼかし (A1)
            // A1 を 1/16 サイズにしてガウスぼかし (A16)
            Shader::Downsample(gaussianA1_sp, gaussianA16_sp);
            Shader::GaussianBlur(gaussianA16_sp, gaussianB16_sp, gaussianA16_sp);
        }

        {
            const ScopedRenderStates2D blend{ BlendState::Additive };

            // マウスオーバーしているならぼかし最大
            gaussianA16_sp.resized(sceneSize).draw(ColorF{(Rect{1090, 720, 120, 80}.contains(Cursor::Pos())? 1.0: Periodic::Jump0_1(2s)) * 3});
        }

    }
}
