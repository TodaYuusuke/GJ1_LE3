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

	//死んだ瞬間
	void InitDeadBody() override;	// 死体
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
		LWP::Math::Vector3 kJumpVelocity = { -4.0f,11.0f,0.0f };	// ジャンプした瞬間の速度
		float kGravity = -0.3f;	// 重力
	}normal_;

	//オーディオがあるフォルダまでのパス
	std::string audioPath_ = "SE/slime/";
	//音量
	float audioVolume_ = 1.0f;
	//走る音
	LWP::Resource::Audio audioJump_;
	LWP::Resource::Audio audioLand_;
	LWP::Resource::Audio audioDead_;


	std::string jumpPath_ = "jump.mp3";
	std::string landPath_ = "stand.mp3";
	std::string deadPath_ = "dead.mp3";
	
	bool isJump_ = false;
};