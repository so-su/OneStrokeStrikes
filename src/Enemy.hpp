#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"

// Enemyを倒したときに得られるスコアを表す構造体
struct Score {
    int32 green;
    int32 red;
    int32 blue;
    int32 green_endpoint;
    int32 red_endpoint;
    int32 blue_endpoint;
};

class Enemy : public Polyomino {
   public:
    Enemy(Point center, bool easy);

    // 初期化する
    void initialize();

    // 外周を計算する
    void compute_perimeter();

    // パスを描画する
    void draw_path() const;

    // カーソルの座標を受け取ってパスを更新
    void update_path(Point cursor_pos);

    // パスを削除する
    void clear_path();

    // 外周のゲージを描画する
    void draw_gauge() const;

    // パスを反転させる
    void reverse_path();

    // ゲージを更新し、満タンになったらtrueを返す
    bool update_gauge();

    // ゲージをリセットする
    void reset_gauge();

    // 攻撃力を返す
    int32 attack_value() const;

    // ゲージの周回速度を上昇させる
    void speed_up_gauge(int32 times);

    // ランダムで消滅する準備をする
    void prepare_to_randomly_vanish();

    // 倒したときにもらえる基礎スコアを計算
    Score get_basic_score() const;

    // 一筆書きのスコアと、一筆書きの色の列をランレングス圧縮したときに分かれる部分の長さの最大値を返す
    std::pair<Score, int32> get_path_score() const;

   private:
    // 一筆書き中のパス
    Array<Point> path;

    // 外周のパス
    Array<Line> perimeter;

    // 外周のゲージの長さ
    double gauge_len;

    // ゲージがたまる速さ
    double gauge_speed;

    // セルの大きさ
    static constexpr int32 cell_size{50};

    // 一筆書きのパスの色
    static constexpr Color path_color{MyColor::Yellow};

    // ゲージの色
    static constexpr Color gauge_color{MyColor::Orange};
};
