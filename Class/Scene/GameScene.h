#pragma once
#include "scene/IScene.h"

#include"../Player/Player.h"
#include"../Drone/Drone.h"
#include"../FollowCamera/FollowCamera.h"
#include "../UI/GameUIManager.h"
#include "../Upgrade/UpgradeManager/UpgradeManager.h"
#include "../Enemy/EnemyManager.h"

#include "../Particle/ParticleManager.h"

class GameScene final
	: public IScene {
public:
	GameScene() = default;
	~GameScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//
	// 背景
	LWP::Primitive::Surface backGround_;

	Player player_;	// プレイヤー
	FollowCamera followCamera_;	// 追従カメラ
	EnemyManager enemyManager_; // 敵管理
	Drone drone_;	// ドローン

	GameUIManager gameUIManager_;	// ゲーム内UIマネージャー
	UpgradeManager upgradeManager_; // アップグレードマネージャー

	LWP::Resource::RigidModel stage_;	// ステージ
	LWP::Object::DirectionLight sun_;	// 太陽
	// パーティクル管理
	ParticleManager particleManager_;

	// ウェーブ数
	int wave_ = 1;

	// デバッグ用
	bool freeMode_ = false;
};