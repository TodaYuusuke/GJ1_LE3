#include "TestScene.h"
#include "Result.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

// 初期化
void TestScene::Initialize() {
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
}

// 更新
void TestScene::Update() {
#if DEMO
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Wave")) {
			ImGui::SliderInt("Begin Wave", &wave_, 1, 10);
			if (ImGui::Button("Start")) { enemyManager_.StartWave(wave_); }
			ImGui::Checkbox("FreeMode", &freeMode_);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	particleManager_.DebugGUI();
#endif

	// ウェーブ終了後処理
	if (enemyManager_.GetEndWave() && !freeMode_) {
		// ウェーブ10終了でゲーム終了
		if (wave_ >= 10) {
			nextSceneFunction = []() { return new Result(); };
		}
		
		// アップグレード確認して次のウェーブへ
		//enemyManager_.StartWave(++wave_);
	}
	// ウェーブ中処理
	else {
		player_.Update();
		followCamera_.Update(player_.GetWorldPosition());
		enemyManager_.Update();
	}

	// いったん外に出す
	drone_.Update();
	gameUIManager_.Update(wave_, enemyManager_.GetRemainingEnemy());
	upgradeManager_.Update();

	// ゲームUIの表示非表示を切り替える
	gameUIManager_.SetIsDisplay(!upgradeManager_.GetIsDisplay());
}