#include "Player.hpp"

Player::Player():hp(1000),max_hp(1000),ap(0),max_ap(1000),sp(0),max_sp(1000),hp_bar(Rect{100,750,800,20},Palette::Green),ap_bar(Rect{950,750,100,20},Palette::Red),sp_bar(Rect{1100,750,100,20},Palette::Blue){
    hp_bar.update(static_cast<double>(hp)/max_hp);
    ap_bar.update(static_cast<double>(ap)/max_ap);
    sp_bar.update(static_cast<double>(sp)/max_sp);
}

void Player::get_damaged(int32 value){
    hp=Max(hp-value,0);
    hp_bar.update(static_cast<double>(hp)/max_hp);
}

void Player::get_healed(int32 value){
    hp=Min(hp+value,max_hp);
    hp_bar.update(static_cast<double>(hp)/max_hp);
}

void Player::get_ap(int32 value){
    ap=Min(ap+value,max_ap);
    ap_bar.update(static_cast<double>(ap)/max_ap);
}

void Player::get_sp(int32 value){
    sp=Min(sp+value,max_sp);
    sp_bar.update(static_cast<double>(sp)/max_sp);
}

bool Player::alive()const{
    return hp>0;
}

bool Player::ap_is_full()const{
    return ap==max_ap;
}

bool Player::sp_is_full()const{
    return sp==max_sp;
}

void Player::reset_ap(){
    ap=0.0;
    ap_bar.update(0.0);
}

void Player::reset_sp(){
    sp=0.0;
    sp_bar.update(0.0);
}

void Player::draw()const{
    hp_bar.draw();
    ap_bar.draw();
    sp_bar.draw();
}
