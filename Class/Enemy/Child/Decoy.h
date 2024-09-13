#pragma once
#include "../IEnemy.h"

class Decoy final
	: public IEnemy {
public:
	Decoy(Player* ptr, LWP::Math::Vector3 summonPos) : IEnemy(ptr, summonPos) { Init(); }
	~Decoy() = default;

	// ** メンバ関数 ** //

	// 初期化
	void ChildInit() override;
	// 更新
	void ChildUpdate() override;

	// デバッグ用GUI
	void DebugGUI() override;
	// 自分のタイプを返す関数
	EnemyType GetType() override { return EnemyType::Spider; }

	/// <summary>
	/// 状態取得
	/// </summary>
	/// <returns></returns>
	IEnemy::Behavior GetBehavior() { return behavior_; }
			

private: // ** ステートパターン ** //

	// 通常
	void InitNormal() override;
	void UpdateNormal() override;

private: // ** メンバ変数 ** //

	// アニメーション
	LWP::Resource::Animation animation_;

private: // ** パラメータ ** //

	//オーディオがあるフォルダまでのパス
	std::string audioPath_ = "SE/spider/";
	//音量
	float audioVolume_ = 0.07f;
	LWP::Resource::Audio audioDead_;

	std::string deadPath_ = "dead1.mp3";
};