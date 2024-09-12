#pragma once
#include <Adapter.h>

class Stage {
public:
	Stage() = default;
	~Stage() = default;

	
	/// <summary>
	/// 親クラスの初期化
	/// </summary>
	void Init(LWP::Object::Camera* camera);


private: // ** メンバ変数 ** //
	// 背景
	LWP::Primitive::Surface backGround_;

	// 下水道モデル
	LWP::Resource::RigidModel stage_;

	// 太陽
	LWP::Object::DirectionLight sun_;
	// ドアのライト
	LWP::Object::PointLight doorLight_[2];
};