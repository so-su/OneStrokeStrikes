#include "AttackRoulette.hpp"

AttackRoulette::AttackRoulette() { initialize(); }

// 初期化する
void AttackRoulette::initialize() {
    std::fill(std::begin(values), std::end(values), initial_value);
    update_angles();
    triangle_angle = 0.0;
    std::fill(std::begin(attack_nums), std::end(attack_nums), 0);

    // ルーレットの各部分の攻撃内容を決める
    for (auto attack_idx : step(3)) {
        if (RandomBool(Shape_rate)) {
            attack_shapes[attack_idx].initialize();
        } else {
            attack_nums[attack_idx] = Random(5, 10);
        }
    }
}

// 画面中央に大きいルーレットを描画
void AttackRoulette::draw() const {
    draw_large_disk();
    draw_icons();
    triangle.rotatedAt(large_circle.center, triangle_angle)
        .draw(MyColor::Orange);
}

// 画面中央に大きいルーレット円盤を描画する
void AttackRoulette::draw_large_disk() const { draw_disk(large_circle); }

// 画面右下に小さいルーレット円盤を描画する
void AttackRoulette::draw_small_disk() const { draw_disk(small_circle); }

// ルーレットが現在のフレームで押され始めたかを返す
bool AttackRoulette::down() const {
    return MouseL.down() and small_circle.contains(Cursor::Pos());
}

// valuesに値を加算する
void AttackRoulette::update_value(int32 green, int32 red, int32 blue) {
    values[0] += green;
    values[1] += red;
    values[2] += blue;
    update_angles();
}

// 針を回転させる
void AttackRoulette::go_around(double speed) {
    triangle_angle += speed * Scene::DeltaTime();

    // 0〜2πの範囲に収める
    triangle_angle = std::fmod(triangle_angle, Math::TwoPi);
}

// ルーレットで選ばれた攻撃の種類を返す
AttackType AttackRoulette::get_attack_type() const {
    if (attack_nums[chosen_index()] > 0) {
        return AttackType::Num;
    } else {
        return AttackType::Shape;
    }
}

// ルーレットで選ばれた攻撃の種類がNumのとき、その数を返す
int32 AttackRoulette::get_attack_num() const {
    return attack_nums[chosen_index()];
}

// ルーレットで選ばれた攻撃の種類がShapeのとき、そのAttackShapeへのポインタを返す
AttackShape* AttackRoulette::get_attack_shape() {
    return &(attack_shapes[chosen_index()]);
}

// ルーレットで選ばれた色を返す
Color AttackRoulette::chosen_color() const {
    const int32 idx{chosen_index()};
    if (idx == 0) {
        return MyColor::Green;
    } else if (idx == 1) {
        return MyColor::Red;
    } else {
        return MyColor::Blue;
    }
}

// 円を指定してルーレット円盤を描画
void AttackRoulette::draw_disk(const Circle& circle) const {
    circle.drawPie(start_angles[0], angles[0], MyColor::Green)
        .drawPie(start_angles[1], angles[1], MyColor::Red)
        .drawPie(start_angles[2], angles[2], MyColor::Blue)
        .drawFrame(circle.r * 0.05, 0, ColorF{0.9});
}

// 画面中央の大きいルーレットの位置に攻撃アイコンを描画
void AttackRoulette::draw_icons() const {
    for (auto attack_idx : step(3)) {
        // アイコンの中心座標
        const Point center{
            (large_circle.center + Vec2{0.0, -large_circle.r / 2})
                .rotateAt(large_circle.center,
                          start_angles[attack_idx] + angles[attack_idx] / 2)
                .asPoint()};

        if (attack_nums[attack_idx] == 0) {  // 攻撃がShape型のとき
            attack_shapes[attack_idx].draw(center, Palette::White);
        } else {  // 攻撃がNum型のとき
            const auto frame_size{AttackShape::cell_size * 3 + 2};
            Rect{Arg::center = center, frame_size, frame_size}.drawFrame(
                0, 2, Palette::White);
            FontAsset(U"Black")(U"{}"_fmt(attack_nums[attack_idx]))
                .drawAt(50, center, Palette::White);
        }
    }
}

// ルーレットの各領域における扇形の角度を更新
void AttackRoulette::update_angles() {
    const double sum = values[0] + values[1] + values[2];
    for (auto i : step(3)) {
        angles[i] = Math::TwoPi * values[i] / sum;
    }

    start_angles[0] = 0.0;
    start_angles[1] = angles[0];
    start_angles[2] = angles[0] + angles[1];
}

// ルーレットで選ばれたインデックス（0,1,2）を返す
int32 AttackRoulette::chosen_index() const {
    if (triangle_angle < start_angles[1]) {
        return 0;
    } else if (triangle_angle < start_angles[2]) {
        return 1;
    } else {
        return 2;
    }
}
