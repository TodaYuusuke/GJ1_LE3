#pragma once
#include <Config.h>

namespace Mask {

    //** マスクフラグを設定している **//

    // プレイヤー
    int Player() { return LWP::Config::Collider::MaskLayer::Player; }
    // 敵
    int Enemy() { return LWP::Config::Collider::MaskLayer::Enemy; }
    // 地面（パーティクル用）
    int Ground() { return LWP::Config::Collider::MaskLayer::Layer2; }
    // 水面
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer3; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer4; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer5; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer6; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer7; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer8; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer9; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer10; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer11; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer12; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer13; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer14; }
    int Layer15() { return LWP::Config::Collider::MaskLayer::Layer15; }
    int ALL() { return LWP::Config::Collider::MaskLayer::ALL; }

}