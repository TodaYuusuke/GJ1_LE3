#include "GameScene.h"
#include "Result.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

// 初期化
void GameScene::Initialize() {
	player_.Initialize();
	followCamera_.Initialize(&mainCamera);
	enemyManager_.Initialize(&player_);
	drone_.Initialize(&player_, &enemyManager_);

	// ゲームUIマネージャーの初期化
	gameUIManager_.Initialize(&player_);

	// 配置物初期化
	stage_.LoadShortPath("Stage/Stage.gltf");	// 下水道
	stage_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);
	sun_.rotation.x = 3.14f;	// 下から照らす
	sun_.color = { 36,42,52,255 };	// 色決定
}

// 更新
void GameScene::Update() {
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
	gameUIManager_.Update();
}