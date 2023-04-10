// 調節可能なパラメータをParameter名前空間にまとめておく
namespace Parameter {
/* ゲームの難易度に関わるもの */
// AlphaEnemyの最大グリッドサイズ
constexpr Size alpha_enemy_max_grid_size_easy{15, 6};
constexpr Size alpha_enemy_max_grid_size_normal{18, 7};
constexpr Size alpha_enemy_max_grid_size_hard{20, 8};

// Enemyの最小グリッドサイズ
constexpr int32 enemy_min_grid_size{3};

// Enemyの最大グリッドサイズ
constexpr int32 enemy_max_grid_size_easy{6};
constexpr int32 enemy_max_grid_size_normal{7};
constexpr int32 enemy_max_grid_size_hard{8};

// Enemyを倒したときの基礎ポイント/セル
constexpr int32 basic_point_per_cell{20};

// 場のEnemyをすべて倒したときのAll Clearボーナスの値
constexpr int32 all_clear_bonus{200};

// AlphaEnemyのゲージの速度
constexpr double alpha_enemy_gauge_speed{1.0};

// Enemyの攻撃力
constexpr int32 enemy_attack_value{100};

// Enemyのリスポーンまでの時間
constexpr double respawn_time{6.0};

// Enemyのゲージの初期速度
constexpr double enemy_gauge_initial_speed{1.0};

// Enemyのゲージの加速度
constexpr double gauge_speed_up_rate_easy{0.3};
constexpr double gauge_speed_up_rate_normal{0.3};
constexpr double gauge_speed_up_rate_hard{0.5};

/* 演出に関わるもの */
// Enemyを一筆書きしてから消滅し始めるまでの時間
constexpr double time_until_vanishing{0.5};

// EnemyをスペシャルやAll Clearで倒したときのリスポーンまでの時間
constexpr double short_respawn_time{2.0};

// ルーレットの最高回転速度
constexpr double roulette_rotation_max_speed{20.0};

// ルーレットの最短回転時間
constexpr double roulette_rotation_min_duration{2.5};

// ルーレットの最長回転時間
constexpr double roulette_rotation_max_duration{4.0};

/* スコアボーナスに関する値*/
// 勝利したときのスコアボーナス
constexpr int32 win_bonus_easy{100000};
constexpr int32 win_bonus_normal{150000};
constexpr int32 win_bonus_hard{200000};

// クリアまでのタイムによるスコアボーナスの倍率
constexpr double time_bonus_rate{200.0};

// 攻撃コンボによるスコアボーナスの倍率
constexpr int32 attack_combo_bonus_rate{10000};

// 一筆書きの色の列をランレングス圧縮したときの長さの最大値の平均によるスコアボーナスの倍率
constexpr double mean_max_run_length_bonus_rate{20000.0};

}  // namespace Parameter
