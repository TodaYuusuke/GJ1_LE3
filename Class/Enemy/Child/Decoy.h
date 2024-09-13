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

private: // ** ステートパターン ** //

	// 通常
	void InitNormal() override;
	void UpdateNormal() override;

private: // ** メンバ変数 ** //

	// アニメーション
	LWP::Resource::Animation animation_;

private: // ** パラメータ ** //

	//移動速度
	float spd_ = 1.8f;

	//歩く時間カウント
	float currentWalkSE_ = 0;
	float walkSEsec_ = 0.5f;

	//オーディオがあるフォルダまでのパス
	std::string audioPath_ = "SE/spider/";
	//音量
	float audioVolume_ = 0.07f;
	float volume_;
	//走る音
	LWP::Resource::Audio audioWalk_;
	LWP::Resource::Audio audioDead_;

	std::string walkPath_ = "walk.mp3";
	std::string deadPath_ = "dead1.mp3";
};