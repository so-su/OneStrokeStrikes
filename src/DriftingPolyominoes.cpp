#pragma once
#include "DriftingPolyominoes.hpp"

DriftingPolyominoes::DriftingPolyominoes() {}

void DriftingPolyominoes::draw() const {
    for (const SimplePolyomino& polyomino : polyominoes) {
        polyomino.draw(timer * 0.25);
    }
}

void DriftingPolyominoes::update() const {
    timer += Scene::DeltaTime();

    // 加わる力を計算する
    Array<Vec2> forces(polyominoes.size(), Vec2{0.0, 0.0});
    for (auto receive_idx : step(polyominoes.size())) {
        // ほかのポリオミノが寄与する力を加える
        for (auto apply_idx : step(polyominoes.size())) {
            if (receive_idx == apply_idx) {
                continue;
            }
            // 上下左右の画面端がつながっているので、力を加える側のポリオミノを
            // 5つに分身させて力を加えることにする
            for (Vec2 delta :
                 {Vec2{0.0, 0.0},
                  Vec2{Scene::Width() + 2 * SimplePolyomino::margin, 0.0},
                  Vec2{0.0, Scene::Height() + 2 * SimplePolyomino::margin},
                  Vec2{-(Scene::Width() + 2 * SimplePolyomino::margin), 0.0},
                  Vec2{0.0,
                       -(Scene::Height() + 2 * SimplePolyomino::margin)}}) {
                Vec2 displacement = polyominoes[receive_idx].center -
                                    polyominoes[apply_idx].center - delta;
                forces[receive_idx] += 1000 * displacement /
                                       displacement.length() *
                                       Math::Exp(-0.01 * displacement.length());
            }
        }

        // マウスが寄与する力を加える
        Vec2 displacement = polyominoes[receive_idx].center - Cursor::PosF();
        forces[receive_idx] += 1000 * displacement / displacement.length() *
                               Math::Exp(-0.01 * displacement.length());
    }

    // 計算した力を加えて更新する
    for (auto poly_idx : step(polyominoes.size())) {
        polyominoes[poly_idx].update(forces[poly_idx]);
    }
}

DriftingPolyominoes::SimplePolyomino::SimplePolyomino(Grid<bool> shape)
    : center{static_cast<Vec2>(RandomVec2(Scene::Rect()))},
      shape(shape),
      base_arg{Random(Math::TwoPi)},
      noise{RandomUint64()},
      noise_in{Random(10.0)},
      blink_period{Random(2.0, 6.0)} {
    arg = base_arg + std::lerp(-Math::Pi, Math::Pi, noise.noise1D(noise_in));
}

// alphaは透過率を制御するファクター
void DriftingPolyominoes::SimplePolyomino::draw(double alpha) const {
    const Point frame_upper_left{center.asPoint() -
                                 shape.size() * cell_size / 2};
    for (Point pos : step(shape.size())) {
        if (not shape[pos]) {
            continue;
        }
        // シーンの開始時は見えず、だんだん透過率が上がるようなファクターをかけておく
        Rect{frame_upper_left + cell_size * pos,
             Size{static_cast<int32>(cell_size * scale_factor),
                  static_cast<int32>(cell_size * scale_factor)}}
            .rotatedAt(center, arg)
            .draw(ColorF{
                1.0, Min(1.0, alpha) *
                         std::lerp(0.2, 0.6, Periodic::Sine0_1(blink_period))});
    }
}

// 外部から受ける力を与え、speed, center, arg を更新
void DriftingPolyominoes::SimplePolyomino::update(Vec2 force) {
    // 速度に線形な抵抗力も加える
    constexpr double resistance{0.1};  // 抵抗力の係数
    speed += (force - resistance * speed) * Scene::DeltaTime();

    // 位置を動かす
    center += speed * Scene::DeltaTime();

    // 画面端にはmarginがあり、その外は上下・左右でそれぞれつながっている
    // marginの外にはみ出したらワープさせる
    if (center.x < -margin) {
        center.x = Scene::Width() + margin;
    } else if (center.x > Scene::Width() + margin) {
        center.x = -margin;
    }
    if (center.y < -margin) {
        center.y = Scene::Height() + margin;
    } else if (center.y > Scene::Height() + margin) {
        center.y = -margin;
    }

    // 回転角の更新
    noise_in += Scene::DeltaTime() * 0.1;
    arg = base_arg + std::lerp(-Math::Pi, Math::Pi, noise.noise1D(noise_in));
}