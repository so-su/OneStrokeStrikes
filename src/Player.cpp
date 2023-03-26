#include "Player.hpp"

Player::Player() {
    hp_bar.update(static_cast<double>(hp) / max_hp);
    ap_bar.update(static_cast<double>(ap) / max_ap);
    sp_bar.update(static_cast<double>(sp) / max_sp);
}

// ダメージを受ける
void Player::get_damaged(int32 value) {
    hp = Max(hp - value, 0);
    hp_bar.update(static_cast<double>(hp) / max_hp);
}

// HPを回復する
void Player::get_healed(int32 value) {
    hp = Min(hp + value, max_hp);
    hp_bar.update(static_cast<double>(hp) / max_hp);
}

// APを得る
void Player::get_ap(int32 value) {
    ap = Min(ap + value, max_ap);
    ap_bar.update(static_cast<double>(ap) / max_ap);
}

// SPを得る
void Player::get_sp(int32 value) {
    sp = Min(sp + value, max_sp);
    sp_bar.update(static_cast<double>(sp) / max_sp);
}

// 生きているかを返す
bool Player::is_alive() const { return hp > 0; }

// APが満タンかを返す
bool Player::ap_is_full() const { return ap == max_ap; }

// SPが満タンかを返す
bool Player::sp_is_full() const { return sp == max_sp; }

// APを0にする
void Player::reset_ap() {
    ap = 0.0;
    ap_bar.update(0.0);
}

// SPを0にする
void Player::reset_sp() {
    sp = 0.0;
    sp_bar.update(0.0);
}

// HP,AP,SPのバーを描画する
void Player::draw() const {
    hp_bar.draw();
    ap_bar.draw();
    sp_bar.draw();
}

// HPバーを描画する
void Player::draw_hp_bar() const{
    hp_bar.draw();
}

// APバーを描画する
void Player::draw_ap_bar() const{
    ap_bar.draw();
}

// SPバーを描画する
void Player::draw_sp_bar() const{
    sp_bar.draw();
}
