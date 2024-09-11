#pragma once
#include "../Player/Player.h"

class HealItem final {
public:
	HealItem() = default;
	~HealItem() = default;

	// ** メンバ関数 ** //
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Init(LWP::Math::Vector3 pos);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private: // ** メンバ変数 ** //

	// モデル
	LWP::Resource::RigidModel model_;
	// コライダー
	LWP::Object::Collider::Collider collider_;

	// 回転速度
	float kRotationSpeed_ = 0.1f;
	// 落下速度
	float kFallSpeed_ = 0.1f;
};