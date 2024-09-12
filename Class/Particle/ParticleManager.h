#pragma once
#include "CeilingFragment.h"

class ParticleManager final {
public:
	ParticleManager() = default;
	~ParticleManager() = default;

	// ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// ImGui
	/// </summary>
	void DebugGUI();


public: // ** パーティクル生成関数 ** //

	/// <summary>
	/// 天井からの破片
	/// </summary>
	void Ceiling(LWP::Math::Vector3 pos);


private: // ** メンバ変数 ** //

	// 地形のコライダー
	LWP::Object::Collider::Collider ground_[4];
	// 天井のコライダー
	LWP::Object::Collider::Collider ceiling_;
	// 水面のコライダー
	LWP::Object::Collider::Collider waterSurface_;

	// パーティクル
	CeilingFragment ceilingFragment_;
	int ceilingFragmentNum_ = 6;	// 生成数
};