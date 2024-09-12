#pragma once
#include <Config.h>

namespace GJMask {

    //** マスクフラグを設定している **//

    // プレイヤー
    static int Player() { return LWP::Config::Collider::MaskLayer::Player; }
    // 敵
    static int Enemy() { return LWP::Config::Collider::MaskLayer::Enemy; }
    // 回復アイテム
    static int HealItem() { return LWP::Config::Collider::MaskLayer::Layer2; }
    // ショットガンの弾
    static int Bullet() { return LWP::Config::Collider::MaskLayer::Layer3; }
    static int Layer4() { return LWP::Config::Collider::MaskLayer::Layer4; }
    static int Layer5() { return LWP::Config::Collider::MaskLayer::Layer5; }
    static int Layer6() { return LWP::Config::Collider::MaskLayer::Layer6; }
    static int Layer7() { return LWP::Config::Collider::MaskLayer::Layer7; }
    static int Layer8() { return LWP::Config::Collider::MaskLayer::Layer8; }
    static int Layer9() { return LWP::Config::Collider::MaskLayer::Layer9; }
    static int Layer10() { return LWP::Config::Collider::MaskLayer::Layer10; }
    static int Layer11() { return LWP::Config::Collider::MaskLayer::Layer11; }
    // パーティクル
    static int Particle() { return LWP::Config::Collider::MaskLayer::Layer12; }
    // 地面（パーティクル用）
    static int Ground() { return LWP::Config::Collider::MaskLayer::Layer13; }
    // 天井（パーティクル用）
    static int Ceiling() { return LWP::Config::Collider::MaskLayer::Layer14; }
    // 水面（パーティクル用）
    static int WaterSurface() { return LWP::Config::Collider::MaskLayer::Layer15; }
    static int ALL() { return LWP::Config::Collider::MaskLayer::ALL; }

}