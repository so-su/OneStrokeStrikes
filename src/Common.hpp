#pragma once
#include <Siv3D.hpp>

// シーンの名前
enum class State {
    Title,
    Game,
    Result,
    Ranking,
};

// 共有するデータ
struct GameData {
    uint32 win;
    
    uint32 enemy;
    
    uint32 green;
    
    uint32 red;
    
    uint32 blue;
    
    uint32 score;
    
    bool easy_mode;
    
    String player_id;
    
    // ResultからRankingに飛んだときにtrueにする
    bool display_player_score=true;
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
