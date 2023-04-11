#include "Player.hpp"

Player::Player() {
    hp_bar.update(static_cast<double>(hp) / max_hp);
    ap_bar.update(static_cast<double>(ap) / max_ap);
    sp_bar.update(static_cast<double>(sp) / max_sp);
}

// 遅延つきHP、AP、SPを更新する
void Player::update_delay_status(){
    // 遅延つきステータスの値を更新
    delay_hp = Math::SmoothDamp(delay_hp, hp, delay_velocity_hp, smooth_time);
    delay_ap = Math::SmoothDamp(delay_ap, ap, delay_velocity_ap, smooth_time);
    delay_sp = Math::SmoothDamp(delay_sp, sp, delay_velocity_sp, smooth_time);
    
    // 変化をバーに反映させる
    // ただし、ほぼ0の場合は厳密に0に更新する
    // ほぼ0になってから、0だと見えるまでにかかる時間が長いため
    {
        const double hp_ratio{delay_hp / max_hp};
        hp_bar.update(hp_ratio < 0.001 ? 0.0 : hp_ratio);
    }
    {
        const double ap_ratio{delay_ap / max_ap};
        ap_bar.update(ap_ratio < 0.001 ? 0.0 : ap_ratio);
    }
    {
        const double sp_ratio{delay_sp / max_sp};
        sp_bar.update(sp_ratio < 0.001 ? 0.0 : sp_ratio);
    }
}

// ダメージを受ける
void Player::get_damaged(int32 value) {
    hp = Max(hp - value, 0);
}

// HPを回復する
void Player::get_healed(int32 value) {
    hp = Min(hp + value, max_hp);
}

// APを得る
void Player::get_ap(int32 value) {
    ap = Min(ap + value, max_ap);
}

// SPを得る
void Player::get_sp(int32 value) {
    sp = Min(sp + value, max_sp);
}

// 生きているかを返す
// 遅延つきHPが追いついてから敗北の判定をする
bool Player::is_alive() const { return delay_hp > 0.1; }

// APが満タンかを返す
bool Player::ap_is_full() const { return ap == max_ap; }

// SPが満タンかを返す
bool Player::sp_is_full() const { return sp == max_sp; }

// APを0にする
void Player::reset_ap() {
    ap = 0;
}

// SPを0にする
void Player::reset_sp() {
    sp = 0;
}

// HP、AP、SPのバーを描画する
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
