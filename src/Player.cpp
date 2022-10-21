#include "Player.hpp"

Player::Player():hp(1000),max_hp(1000),ap(0),max_ap(1000),sp(0),max_sp(1000),hp_bar{Rect{100,750,800,20}}{
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
}

void Player::get_sp(int32 value){
    sp=Min(sp+value,max_sp);
}

bool Player::alive()const{
    return hp>0;
}

void Player::draw()const{
    hp_bar.draw();
}
