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
};

using App = SceneManager<State, GameData>;
