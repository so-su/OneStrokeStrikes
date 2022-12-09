#pragma once
#include <Siv3D.hpp>

// シーンの名前
enum class State {
    Title,
    Game,
    Result,
};

// 共有するデータ
struct GameData {
    bool win;
    int32 win_cnt=0;
};

using App = SceneManager<State, GameData>;

// 色
namespace MyColor{
    constexpr Color Green=Color{53, 161, 107};
    constexpr Color Red=Color{255, 40, 0};
    constexpr Color Blue=Color{0, 65, 255};
    constexpr Color Yellow=Color{250,245,0};
    constexpr Color Orange=Color{255,153,0};
}
