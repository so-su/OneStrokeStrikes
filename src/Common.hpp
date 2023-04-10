#pragma once
#include <Siv3D.hpp>

#include "Parameter.hpp"

// シーンの名前
enum class State {
    Title,
    Game,
    Result,
    Ranking,
};

enum class Difficulty {
    Easy,
    Normal,
    Hard,
};

// 共有するデータ
struct GameData {
    // ランキングに登録するゲームのスコア
    int32 score;
    
    // 勝利したかどうか
    bool win;
    
    // ゲームの経過時間（秒）
    double elapsed_time;

    // Enemyを倒して得たポイントの総和
    int32 point_sum;
    
    // 図形による攻撃のコンボの回数
    int32 attack_combo;
    
    // 一筆書きの色の列をランレングス圧縮したときの長さの最大値の総和
    int32 sum_max_run_length;
    
    // 一筆書きが成功した回数
    int32 cnt_one_stroke;

    // 「ちょっとあそぶ」モード
    Difficulty difficulty{Difficulty::Easy};

    // プレイヤーのユーザーid
    String player_id;

    // ResultからRankingに飛んだときにtrueにする
    bool display_player_score{false};
};

using App = SceneManager<State, GameData>;

// 色
namespace MyColor {
constexpr Color Green = Color{53, 161, 107};
constexpr Color Red = Color{255, 40, 0};
constexpr Color Blue = Color{0, 65, 255};
constexpr Color Yellow = Color{250, 245, 0};
constexpr Color Orange = Color{255, 153, 0};
constexpr Color White = ColorF{0.9};
constexpr Color Background = Color{0, 0, 50};
constexpr Color Forward = Palette::Skyblue;
constexpr Color Backward = ColorF{0.85};
}  // namespace MyColor
