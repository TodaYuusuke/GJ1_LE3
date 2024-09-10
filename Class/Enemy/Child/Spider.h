#pragma once
#include "../IEnemy.h"

class Spider final
	: public IEnemy {
public:
	Spider(Player* ptr) : IEnemy(ptr) { Init(); }
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
	float spd_ = 5.0f;
};