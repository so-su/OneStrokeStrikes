#include "Player.hpp"

Player::Player():max_hp(1000),hp(1000),attack{20},defence{10},hp_bar{Rect{100,200,200,10}}{
    
}

void Player::get_damaged(int32 damage){
    hp=Max(hp-damage,0);
    hp_bar.update((double)hp/max_hp);
}

bool Player::alive()const{
    return hp>0;
}

int32 Player::attack_value()const{
    return attack;
}

void Player::draw()const{
    hp_bar.draw();
}
