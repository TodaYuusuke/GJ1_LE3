#include "GameScene.h"
#include "Result.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

// 初期化
void GameScene::Initialize() {
	// パーティクル初期化
	particleManager_.Init();
	
	// 黒背景
	backGround_.worldTF.translation.z = 1000.0f;
	backGround_.worldTF.scale = { 3000.0f,3000.0f,1.0f };
	backGround_.worldTF.Parent(&mainCamera.transform);
	backGround_.material.enableLighting = false;
	backGround_.material.color = Utility::ColorPattern::BLACK;

	player_.Initialize(&particleManager_);
	followCamera_.Initialize(&mainCamera);
	enemyManager_.Initialize(&player_);
	drone_.Initialize(&player_, &enemyManager_);

	// ゲームUIマネージャーの初期化
	gameUIManager_.Initialize(&player_);
	// アップグレードマネージャーの初期化
	upgradeManager_.Initialize(&player_, &drone_);

	// 配置物初期化
	stage_.LoadShortPath("Stage/Stage.gltf");	// 下水道
	stage_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);
	sun_.rotation.x = 3.14f;	// 下から照らす
	sun_.color = { 36,42,52,255 };	// 色決定
}

// 更新
void GameScene::Update() {
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