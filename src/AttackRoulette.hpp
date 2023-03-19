#pragma once
#include "AttackShape.hpp"
#include "Common.hpp"

// 攻撃の種類を表すクラス
enum class AttackType {
    Shape, // 図形で攻撃して重なった部分を破壊
    Num, // 指定の数だけ破壊
};

class AttackRoulette {
   public:
    AttackRoulette();

    // 初期化する
    void initialize();

    // 画面中央に大きいルーレットを描画
    void draw() const;

    // 画面中央に大きいルーレット円盤を描画する
    void draw_large_disk() const;
    
    // 画面右下に小さいルーレット円盤を描画する
    void draw_small_disk() const;

    // ルーレットが現在のフレームで押され始めたかを返す
    bool down() const;

    // valuesに値を加算する
    void update_value(int32 green, int32 red, int32 blue);

    // 針を回転させる
    void go_around(double speed);

    // ルーレットで選ばれた攻撃の種類を返す
    AttackType get_attack_type() const;

    // ルーレットで選ばれた攻撃の種類がNumのとき、その数を返す
    int32 get_attack_num() const;

    // ルーレットで選ばれた攻撃の種類がShapeのとき、そのAttackShapeへのポインタを返す
    AttackShape* get_attack_shape();

    // ルーレットで選ばれた色を返す
    Color chosen_color() const;

   private:
    // 円を指定して円盤を描画
    void draw_disk(const Circle& circle) const;
    
    // 大きいルーレットの位置に攻撃アイコンを描画
    void draw_icons() const;
    
    // ルーレットの各領域における扇形の角度を更新
    void update_angles();
    
    // ルーレットで選ばれたインデックス（0,1,2）を返す
    int32 chosen_index() const;

    // AttackShapeの配列
    std::array<AttackShape, 3> attack_shapes;

    // Num型の攻撃を表す配列
    std::array<int32, 3> attack_nums;

    // valuesの値の比が、ルーレットの各部分が占める割合になる
    std::array<int32, 3> values;

    // 各領域の扇形が始まる角度
    std::array<double, 3> start_angles;

    // 各領域の扇形が占める角度
    std::array<double, 3> angles;

    // 針が指している角度
    double triangle_angle;
    
    // 画面中央に描画される大きいルーレットの円
    static constexpr Circle large_circle{700, 500, 150};

    // 画面右下に描画される小さいルーレットの円
    static constexpr Circle small_circle{1280, 760, 25};

    // ルーレットの針を表す三角形
    static constexpr Triangle triangle{680, 330, 720, 330, 700, 400};
    
    // valuesに入る初期値
    // この値が大きいほど割合が変動しにくい
    static constexpr int32 initial_value{10};
    
    // AttackTypeがShapeになる確率
    static constexpr double Shape_rate{0.5};
};
