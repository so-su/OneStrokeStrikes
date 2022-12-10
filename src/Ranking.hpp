#pragma once
#include "Common.hpp"

// ランキングシーン
class Ranking : public App::Scene {
   public:
    Ranking(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    
    bool get_ranking();
    
    void load_ranking();
    
    void draw_ranking()const;
    
    struct User{
        String user_id;
        uint32 score;
    };
    
    Array<User> ranking;
    
    std::array<Rect,10> rects;
    
    const URL  url=U"https://script.google.com/macros/s/AKfycbzaiBu8G7tMH6fxKK9iysd5wVD03pVLvqPbDZIq5ZzNveKAqA7DkdKWStjfwwXiY73c/exec";
    
    const HashTable<String, String> headers = {};
    
    const FilePath save_file_path = U"ranking.json";
};
