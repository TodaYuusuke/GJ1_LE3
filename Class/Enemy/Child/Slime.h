#pragma once
#include "../IEnemy.h"

class Slime final
	: public IEnemy {
public:
	Slime(Player* ptr, LWP::Math::Vector3 summonPos) : IEnemy(ptr, summonPos) { Init(); }
	~Slime() = default;

	// ** メンバ関数 ** //

	// 初期化
	void ChildInit() override;
	// 更新
	void ChildUpdate() override;

	// デバッグ用GUI
	void DebugGUI() override;
	// 自分のタイプを返す関数
	EnemyType GetType() override { return EnemyType::Slime; }


private: // ** ステートパターン ** //

	// 通常
	void InitNormal() override;
	void UpdateNormal() override;
	// ノックバック
	void InitKnockback() override;


private: // ** パラメーター ** //

	// 通常（ジャンプ用パラメータ）
	struct Normal {
		float jumpInterval = 0.0f;	// ジャンプインターバル
		LWP::Math::Vector3 velocity = { 0.0f,0.0f,0.0f };	// 速度

		float kJumpIntervalTime = 4.0f;	// ジャンプインターバル時間（秒）
		LWP::Math::Vector3 kJumpVelocity = { -0.06f,0.15f,0.0f };	// ジャンプした瞬間の速度
		float kGravity = -0.002f;	// 重力
	}normal_;
};