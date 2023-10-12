#pragma once
#include "Common.hpp"

class DriftingPolyominoes {
   public:
    DriftingPolyominoes();

    void draw() const;

    // Titleシーンのdraw関数から呼び出したいので、constにしておかないとだめ
    // その関係でメンバ変数もmutableにする必要がある
    void update() const;

   private:
    // シーン開始からの経過時間(秒)
    // Scene::Time() は何度も同じシーンに遷移すると累積していくため使えない
    mutable double timer{0.0};

    // 描画するポリオミノひとつを表すクラス
    class SimplePolyomino {
       public:
        SimplePolyomino(Grid<bool> shape);

        // alphaは透過率を制御するファクター
        void draw(double alpha) const;

        // 外部から受ける力を与え、speed, center, arg を更新
        void update(Vec2 force);

        // 画面の外の余白の大きさ
        static constexpr int32 margin{100};

        // 図形の中心座標
        Vec2 center;

       private:
        // 実際のセルの大きさ = cell_size * scale_factor
        static constexpr int32 cell_size{40};
        static constexpr double scale_factor{0.85};

        const Grid<bool> shape;

        Vec2 speed;

        // base_argを中心にしてargを動かす
        const double base_arg;
        double arg;

        // argを動かすときに使うパーリンノイズ
        const PerlinNoise noise;
        double noise_in;

        // チカチカの周期
        const double blink_period;
    };

    // 12個のペントミノたち
    mutable Array<SimplePolyomino> polyominoes{
        Grid{{true, true, false}, {false, true, true}, {false, true, false}},
        Grid{{true, true, true, true}, {true, false, false, false}},
        Grid{{true, true, true, false}, {false, false, true, true}},
        Grid{{true, true, true}, {true, true, false}},
        Grid{{true, true, true, true}, {false, true, false, false}},
        Grid{{true, true, false}, {false, true, false}, {false, true, true}},
        Grid{{true, true, true}, {false, true, false}, {false, true, false}},
        Grid{{true, true, true}, {true, false, true}},
        Grid{{true, true, true}, {true, false, false}, {true, false, false}},
        Grid{{
                 false,
                 true,
                 true,
             },
             {true, true, false},
             {true, false, false}},
        Grid{{true, true, true, true, true}},
        Grid{{false, true, false}, {true, true, true}, {false, true, false}},
    };
};
