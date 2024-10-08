#pragma once
#include "CeilingFragment.h"
#include "JumpFragment.h"
#include "MilkCrown.h"

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
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui
	/// </summary>
	void DebugGUI();


public: // ** パーティクル生成関数 ** //

	/// <summary>
	/// 天井からの破片
	/// </summary>
	void Ceiling(LWP::Math::Vector3 pos);
	/// <summary>
	/// ジャンプ時の破片
	/// </summary>
	void Jump(LWP::Math::Vector3 pos);


private: // ** メンバ変数 ** //

	// 地形のコライダー
	LWP::Object::Collider::Collider ground_[4];
	// 天井のコライダー
	LWP::Object::Collider::Collider ceiling_;
	// 水面のコライダー
	LWP::Object::Collider::Collider waterSurface_;

	// 天井破片
	CeilingFragment ceilingFragment_;
	int ceilingFragmentNum_ = 11;	// 生成数
	// ジャンプ破片
	JumpFragment jumpFragment_;
	int jumpFragmentNum_ = 6;	// 生成数
	// 水面表現
	MilkCrown milkCrown_;
	int milkCrownNum_ = 12;	// 生成数
	LWP::Resource::Audio waterSE_;
};