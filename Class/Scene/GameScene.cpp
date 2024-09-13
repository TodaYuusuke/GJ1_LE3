#include "GameScene.h"
#include "Result.h"
#include "NullScene.h"

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

	// BGM
	bgm_.Load("BGM/game1.mp3");
	bgm_.Play(0.1f, 255);

	fade_.Init();
}

// 更新
void GameScene::Update() {
	// UIの遅延用
	if (uiInitFlag == 0) {
		gameUIManager_.SetUp();
		uiInitFlag++;
	}
	else if(uiInitFlag == -1) {
		uiInitFlag++;
	}

	// フェードインアウト更新
	fade_.Update();
	if (fade_.GetOut()) {
		bgm_.Stop();
		nextSceneFunction = []() { return new NullScene([]() { return new Result(); }); };
	}
	// フェードインが終わるまで処理しない
	if (!fade_.GetIn()) {
		return;
	}

	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		fade_.Out();
	}



	// 死亡演出
	if (deadStaging_.flag) {
		// 時間更新
		deadStaging_.time += Info::GetDeltaTimeF();
		if (deadStaging_.time > deadStaging_.totalTime) {
			deadStaging_.time = deadStaging_.totalTime;
			fade_.Out();
		}

		mainCamera.pp.grayScale.intensity = ResultLerp(0.0f, deadStaging_.grayInt, deadStaging_.time / deadStaging_.totalTime);
		mainCamera.pp.vignetting.intensity = ResultLerp(0.0f, deadStaging_.vignetInt, deadStaging_.time / deadStaging_.totalTime);
	}
	// 通常処理
	else {
		// ウェーブが終わったかを確認
		if (!waveEnd_ && enemyManager_.GetEndWave()) {
			waveEnd_ = true;
			// スキルポイント+1
			upgradeManager_.AddSkilPoint(1);
			upgradeManager_.SetIsDisplay(true);
		}

		// ウェーブ終了後処理
		if (waveEnd_) {
			// ウェーブ10終了でゲーム終了
			if (wave_ >= 10) {
				bgm_.Stop();
				nextSceneFunction = []() { return new NullScene([]() { return new Result(); }); };
				upgradeManager_.SetIsDisplay(false);	// アップグレードUIを表示しない
			}
			else {
				// アップグレードが終わったか確認
				if (!upgradeManager_.GetIsDisplay()) {
					// アップグレード確認して次のウェーブへ
					gameUIManager_.SetUp();	// UI更新
					enemyManager_.StartWave(++wave_);
					waveEnd_ = false;
				}
			}
		}
		// ウェーブ中処理
		else {
			player_.Update();
			followCamera_.Update(player_.GetWorldPosition());
			enemyManager_.Update();
			drone_.Update();

			// 死亡チェック
			if (!player_.GetIsActive()) {
				// 演出開始
				deadStaging_.flag = true;
			}
		}

		// 外で更新
		gameUIManager_.Update(wave_, enemyManager_.GetRemainingEnemy());
		upgradeManager_.Update();
		// ゲームUIの表示非表示を切り替える
		gameUIManager_.SetIsDisplay(!upgradeManager_.GetIsDisplay());
	}
}