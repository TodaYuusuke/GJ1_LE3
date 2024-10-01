#pragma once
#include <Config.h>

namespace GJMask {

    //** マスクフラグを設定している **//

    // プレイヤー
    static int Player() { return LWP::Config::Collider::MaskLayer::Player; }
    // 敵
    static int Enemy() { return LWP::Config::Collider::MaskLayer::Enemy; }
    // 回復アイテム
    static int HealItem() { return LWP::Config::Collider::MaskLayer::Layer5; }
    // ショットガンの弾
    static int Bullet() { return LWP::Config::Collider::MaskLayer::Bullet; }
    // パーティクル
    static int Particle() { return LWP::Config::Collider::MaskLayer::Particle; }
    // 地面（パーティクル用）
    static int Ground() { return LWP::Config::Collider::MaskLayer::Ground; }
    // 天井（パーティクル用）
    static int Ceiling() { return LWP::Config::Collider::MaskLayer::Layer14; }
    // 水面（パーティクル用）
    static int WaterSurface() { return LWP::Config::Collider::MaskLayer::Layer15; }
    static int ALL() { return LWP::Config::Collider::MaskLayer::ALL; }

}