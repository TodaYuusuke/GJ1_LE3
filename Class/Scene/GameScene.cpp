#include "GameScene.h"
#include "Result.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

// 初期化
void GameScene::Initialize() {
	// ステージ初期化
	stage_.Init(&mainCamera);
	// パーティクル初期化
	particleManager_.Init();

	player_.Initialize(&particleManager_);
	followCamera_.Initialize(&mainCamera);
	enemyManager_.Initialize(&player_);
	drone_.Initialize(&player_, &enemyManager_);

	// ゲームUIマネージャーの初期化
	gameUIManager_.Initialize(&player_);
	// アップグレードマネージャーの初期化
	upgradeManager_.Initialize(&player_, &drone_);

	// Wave1スタート
	enemyManager_.StartWave(wave_);
}

// 更新
void GameScene::Update() {
	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		nextSceneFunction = []() { return new Result(); };
	}

	// ウェーブ終了後処理
	if (enemyManager_.GetEndWave()) {
		// ウェーブ10終了でゲーム終了
		if (wave_ >= 10) {
			nextSceneFunction = []() { return new Result(); };
		}
		else {
			// スキルポイント+1
			upgradeManager_.AddSkilPoint(1);
			upgradeManager_.SetIsDisplay(true);
			
			// アップグレード確認して次のウェーブへ
			//enemyManager_.StartWave(++wave_);
		}
	}
	// ウェーブ中処理
	else {
		player_.Update();
		followCamera_.Update(player_.GetWorldPosition());
		enemyManager_.Update();
		drone_.Update();
	}

	// いったん外に出す
	gameUIManager_.Update();
	upgradeManager_.Update();

	// ゲームUIの表示非表示を切り替える
	gameUIManager_.SetIsDisplay(!upgradeManager_.GetIsDisplay());
}