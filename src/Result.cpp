#include "Result.hpp"

Result::Result(const InitData& init) : IScene{init} {
    // 勝利によるスコアボーナス
    if(getData().win){
        if(getData().easy_mode){
            getData().score += Parameter::win_bonus_easy;
        }
        else{
            getData().score += Parameter::win_bonus;
        }
    }
    
    // クリアまでのタイムによるスコアボーナス
    getData().score += static_cast<int32>((1000.0 - getData().elapsed_time) * Parameter::time_bonus_rate);
    
    // ためたポイントの総和によるスコアボーナス
    getData().score += getData().point_sum;
    
    // 図形による攻撃のコンボによるスコア
    getData().score += getData().attack_combo * Parameter::attack_combo_bonus_rate;
    
    // 一筆書きの色の列をランレングス圧縮したときの平均の長さによるスコアボーナス
    mean_run_length = static_cast<double>(getData().sum_run_length) / getData().num_run_length_parts;
    getData().score += static_cast<int32>((mean_run_length - 1.0) * Parameter::mean_run_length_bonus_rate);
}

void Result::update() {
    // ボタンの更新
    if (see_ranking.update(can_press_button)) {
        can_press_button = false;
    }
    if (back_to_title.update(can_press_button)) {
        can_press_button = true;
    }

    // ボタンが押されていて、かつゲージが満タンの状態のとき
    if (see_ranking.completed()) {
        getData().display_player_score = true;
        changeScene(State::Ranking);
    }
    if (back_to_title.completed()) {
        getData().display_player_score = false;
        changeScene(State::Title);
    }
}

void Result::draw() const {
    Scene::SetBackground(MyColor::White);

    FontAsset(U"Kaisotai")(U"リザルト").drawAt(80, 700, 100, Palette::Black);
    frame.drawFrame(1, 1, Palette::Dimgray);
    
    FontAsset(U"Black")(U"トータルスコア  {}"_fmt(getData().score))
            .drawAt(50, 700, 230, Palette::Black);

    if(getData().easy_mode){
        FontAsset(U"Regular")(U"みならいモード").draw(Arg::leftCenter = Point{left_center, top}, Palette::Black);
    }
    else{
        FontAsset(U"Regular")(U"しょくにんモード").draw(Arg::leftCenter = Point{left_center, top}, Palette::Black);
    }
    
    if(getData().win){
        FontAsset(U"Regular")(U"クリア！").draw(Arg::rightCenter = Point{right_center, top}, Palette::Black);
    }
    else{
        FontAsset(U"Regular")(U"ゲームオーバー...").draw(Arg::rightCenter = Point{right_center, top}, Palette::Black);
    }
    
    if(getData().win){
        FontAsset(U"Regular")(U"クリアまでの秒数").draw(Arg::leftCenter = Point{left_center, top + line_space}, Palette::Black);
        FontAsset(U"Regular")(U"{}"_fmt(static_cast<int32>(getData().elapsed_time)))
                    .draw(Arg::rightCenter = Point{right_center, top + line_space}, Palette::Black);
    }
    else{
        FontAsset(U"Regular")(U"クリアまでの秒数").draw(Arg::leftCenter = Point{left_center, top + line_space}, Palette::Black);
        FontAsset(U"Regular")(U"ー").draw(Arg::rightCenter = Point{right_center, top + line_space}, Palette::Black);
    }
    
    FontAsset(U"Regular")(U"ためたポイント")
        .draw(Arg::leftCenter = Point{left_center, top + 2 * line_space}, Palette::Black);
    FontAsset(U"Regular")(U"{}"_fmt(getData().point_sum)).draw(Arg::rightCenter = Point{right_center, top + 2 * line_space}, Palette::Black);
    
    FontAsset(U"Regular")(U"攻撃コンボの回数")
        .draw(Arg::leftCenter = Point{left_center, top + 3 * line_space}, Palette::Black);
    FontAsset(U"Regular")(U"{}"_fmt(getData().attack_combo)).draw(Arg::rightCenter = Point{right_center, top + 3 * line_space}, Palette::Black);
    
    FontAsset(U"Regular")(U"同じ色を連続させた平均長さ")
        .draw(Arg::leftCenter = Point{left_center, top + 4 * line_space}, Palette::Black);
    FontAsset(U"Regular")(U"{:.2f}"_fmt(mean_run_length)).draw(Arg::rightCenter = Point{right_center, top + 4 * line_space}, Palette::Black);

    see_ranking.draw();
    FontAsset(U"Regular")(U"ランキングをみる")
            .drawAt(20, 550, 700, Palette::Black);

    back_to_title.draw();
    FontAsset(U"Regular")(U"タイトルにもどる")
        .drawAt(20, 850, 700, Palette::Black);
}
