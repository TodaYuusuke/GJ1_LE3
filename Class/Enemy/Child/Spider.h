#pragma once
#include "../IEnemy.h"

class Spider final
	: public IEnemy {
public:
	Spider(Player* ptr) : IEnemy(ptr) { Initialize(); }
	~Spider() = default;

	// ** メンバ関数 ** //

	// 初期化
	void Initialize() override;
	// 更新
	void ChildUpdate() override;

	// 自分のタイプを返す関数
	EnemyType GetType() override { return EnemyType::Spider; }


private: // ** ステートパターン ** //

	// 通常
	void InitNormal() override;
	void UpdateNormal() override;


private: // ** メンバ変数 ** //

	// アニメーション
	LWP::Resource::Animation animation_;
	// 当たり判定
	LWP::Object::Collider::Collider collider_;

private: // ** パラメータ ** //

	//移動速度
	float spd_ = 5.0f;
};