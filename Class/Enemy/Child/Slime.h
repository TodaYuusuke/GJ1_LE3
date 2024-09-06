#pragma once
#include "../IEnemy.h"

class Slime final
	: public IEnemy {
public:
	Slime(Player* ptr) : IEnemy(ptr) { Initialize(); }
	~Slime() = default;

	// ** メンバ関数 ** //

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// 自分のタイプを返す関数
	EnemyType GetType() override { return EnemyType::Slime; }

	void DebugGUI() override;
private: // ** メンバ変数 ** //

	LWP::Object::Collider::Collider collider_;

	//飛ぶまでのカウント
	float jumpCount_ = 0;

private: // ** パラメータ ** //

	//飛んだかどうかのフラグ
	bool isJump_ = false;

	LWP::Math::Vector3 velo_;

	//次飛ぶまでの秒数
	float maxjumpTime_ = 2.0f;

	//Y軸の傾きの強さ
	float jumpYNum_ = 2.0f;

	//
	float jumpSpd_ = 50.0f;

	//落ちる速度
	float gravity_ = -90.0f;
};