#pragma once
#include "scene/IScene.h"

#include"../Player/Player.h"
#include"../Drone/Drone.h"
#include"../FollowCamera/FollowCamera.h"
#include "../UI/GameUIManager.h"
#include "../Upgrade/UpgradeManager/UpgradeManager.h"
#include "../Enemy/EnemyManager.h"

#include "../Particle/ParticleManager.h"
#include "../Stage/Stage.h"
#include "FadeManager.h"

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
	Player player_;	// プレイヤー
	FollowCamera followCamera_;	// 追従カメラ
	EnemyManager enemyManager_; // 敵管理
	Drone drone_;	// ドローン

	GameUIManager gameUIManager_;	// ゲーム内UIマネージャー
	UpgradeManager upgradeManager_; // アップグレードマネージャー

	// ステージ
	Stage stage_;
	// パーティクル管理
	ParticleManager particleManager_;

	// ウェーブ数
	int wave_ = 1;
	// ウェーブ終了フラグ
	bool waveEnd_ = false;

	// UI初期化用
	int uiInitFlag = -1;

	// BGM
	LWP::Resource::Audio bgm_;

	// 死亡時の演出
	struct DeadStaging {
		// 演出開始
		bool flag = false;

		// 演出にかかる時間
		float totalTime = 1.5f;
		// 経過時間
		float time = 0.0f;

		// グレースケールの強度
		float grayInt = 1.0f;
		// ビネットの強度
		float vignetInt = 16.0f;
	}deadStaging_;

	// フェードインアウト
	FadeManager fade_;
};