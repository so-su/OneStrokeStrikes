#include "Ranking.hpp"

Ranking::Ranking(const InitData& init) : IScene{init} {
    get_ranking();
    load_ranking();
    
    for(auto rank:step(10)){
        rects[rank]=Rect{400,200+50*rank,600,50};
    }
}

void Ranking::update(){
    
}

void Ranking::draw() const{
    FontAsset(U"Kaisotai")(U"ランキング").drawAt(80,700,100,Palette::Black);
    
    draw_ranking();
}

bool Ranking::get_ranking(){
    if (SimpleHTTP::Get(url, headers, save_file_path)){
        return true;
    }
    return false;
}

void Ranking::load_ranking(){
    const JSON users = JSON::Load(U"ranking.json");
    for(const auto& user:users.arrayView()){
        ranking.emplace_back(User{user[U"user_id"].get<String>(),user[U"score"].get<uint32>()});
    }
}

void Ranking::draw_ranking()const{    
    for(auto rank:step(10)){
        rects[rank].drawFrame(1,1,Palette::Dimgray);
        FontAsset(U"Kaisotai")(rank+1).draw(40,Arg::rightCenter=Vec2{450,200+50*rank+25},Palette::Black);
    }
    
    for(auto rank:step(std::size(ranking))){
        const auto user=ranking[rank];
        FontAsset(U"Regular")(user.user_id).draw(Arg::leftCenter=Vec2{500,200+50*rank+25},Palette::Black);
        FontAsset(U"Regular")(user.score).draw(Arg::rightCenter=Vec2{980,200+50*rank+25},Palette::Black);
    }
}
