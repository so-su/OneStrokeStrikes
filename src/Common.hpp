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
    // 直前のゲームのスコア
    Optional<int32> score;
};

using App = SceneManager<State, GameData>;
