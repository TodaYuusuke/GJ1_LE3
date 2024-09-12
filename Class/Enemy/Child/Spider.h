#pragma once
#include "../IEnemy.h"

class Spider final
	: public IEnemy {
public:
	Spider(Player* ptr, LWP::Math::Vector3 summonPos) : IEnemy(ptr, summonPos) { Init(); }
	~Spider() = default;

	// ** メンバ関数 ** //

	// 初期化
	void ChildInit() override;
	// 更新
	void ChildUpdate() override;

	// デバッグ用GUI
	void DebugGUI() override;
	// 自分のタイプを返す関数
	EnemyType GetType() override { return EnemyType::Spider; }

	//死んだ瞬間
	void InitDeadBody() override;	// 死体
private: // ** ステートパターン ** //

	// 通常
	void InitNormal() override;
	void UpdateNormal() override;
	// ノックバック
	void InitKnockback() override;
	// 死亡アニメーション
	void InitDying();

private: // ** メンバ変数 ** //

	// アニメーション
	LWP::Resource::Animation animation_;

private: // ** パラメータ ** //

	//移動速度
	float spd_ = 1.8f;

	//オーディオがあるフォルダまでのパス
	std::string audioPath_ = "SE/spider/";
	//音量
	float audioVolume_ = 1.0f;
	//走る音
	LWP::Resource::Audio audioWalk_;
	LWP::Resource::Audio audioDead_;


	std::string walkPath_ = "walk.mp3";
	std::string deadPath_ = "dead1.mp3";


};