#include "Result.hpp"

Result::Result(const InitData& init) : IScene{init} {
    // 勝利によるスコアボーナス
    if(getData().win){
        if(getData().difficulty == Difficulty::Easy){
            getData().score += Parameter::win_bonus_easy;
        }
        else if(getData().difficulty == Difficulty::Normal){
            getData().score += Parameter::win_bonus_normal;
        }
        else{
            getData().score += Parameter::win_bonus_hard;
        }
    }
    
    // クリアまでのタイムによるスコアボーナス
    if(getData().win){
        getData().score += static_cast<int32>((1000.0 - getData().elapsed_time) * Parameter::time_bonus_rate);
    }
    
    // ためたポイントの総和によるスコアボーナス
    getData().score += getData().point_sum;
    
    // 図形による攻撃のコンボによるスコア
    getData().score += getData().attack_combo * Parameter::attack_combo_bonus_rate;
    
    // 一筆書きの色の列をランレングス圧縮したときの長さの最大値の平均によるスコアボーナス
    if(getData().cnt_one_stroke > 0){
        mean_max_run_length = static_cast<double>(getData().sum_max_run_length) / getData().cnt_one_stroke;
    }
    getData().score += static_cast<int32>((mean_max_run_length - 1.0) * Parameter::mean_max_run_length_bonus_rate);
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

    {
        String difficulty_text;
        if(getData().difficulty == Difficulty::Easy){
            difficulty_text=U"みならいモード";
        }
        else if(getData().difficulty == Difficulty::Normal){
            difficulty_text=U"じゅくれんモード";
        }
        else{
            difficulty_text=U"しょくにんモード";
        }
        FontAsset(U"Regular")(difficulty_text).draw(Arg::leftCenter = Point{left_center, top}, Palette::Black);
    }
    
    if(getData().win){
        FontAsset(U"Black")(U"クリア！").draw(30, Arg::rightCenter = Point{right_center, top}, Palette::Black);
    }
    else{
        FontAsset(U"Black")(U"ゲームオーバー...").draw(30, Arg::rightCenter = Point{right_center, top}, Palette::Black);
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
    
    FontAsset(U"Regular")(U"ぴったりコンボの回数")
        .draw(Arg::leftCenter = Point{left_center, top + 3 * line_space}, Palette::Black);
    FontAsset(U"Regular")(U"{}"_fmt(getData().attack_combo)).draw(Arg::rightCenter = Point{right_center, top + 3 * line_space}, Palette::Black);
    
    FontAsset(U"Regular")(U"一筆書きパワー")
        .draw(Arg::leftCenter = Point{left_center, top + 4 * line_space}, Palette::Black);
    FontAsset(U"Regular")(U"{:.2f}"_fmt(mean_max_run_length)).draw(Arg::rightCenter = Point{right_center, top + 4 * line_space}, Palette::Black);

    see_ranking.draw();
    FontAsset(U"Regular")(U"ランキングをみる")
            .drawAt(20, 550, 700, Palette::Black);

    back_to_title.draw();
    FontAsset(U"Regular")(U"タイトルにもどる")
        .drawAt(20, 850, 700, Palette::Black);
}
