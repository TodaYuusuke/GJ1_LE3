#include "TestScene.h"

// 初期化
void TestScene::Initialize() {
	player_.Initialize();
	mainCamera.transform.translation.z = -100;
	mainCamera.transform.translation.y = 10;

	enemyManager_.Initialize(&player_);
}

// 更新
void TestScene::Update() {
	//mainCamera.DebugGUI();

	std::vector<BulletData*> data = player_.GetPlayerBulletsData();

	player_.Update();

	enemyManager_.Update();
}