#include "TestScene.h"

// 初期化
void TestScene::Initialize() {
	player_.Initialize();
	mainCamera.transform.translation.z = -100;
	mainCamera.transform.translation.y = 10;

	enemyManager_.Initialize(&player_);

	followCamera_.Initialize(&mainCamera);
}

// 更新
void TestScene::Update() {
	player_.Update();
	followCamera_.Update(player_.GetWorldPosition());
	enemyManager_.Update();
}