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


public: // ** パーティクル生成関数 ** //

	/// <summary>
	/// 天井からの破片
	/// </summary>
	void Ceiling(LWP::Math::Vector3 pos);


private: // ** メンバ変数 ** //

	// パーティクル
	CeilingFragment ceilingFragment_;
};