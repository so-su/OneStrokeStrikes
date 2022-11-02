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
