#include "TestScene.h"

// 初期化
void TestScene::Initialize() {
	player_.Initialize();
	mainCamera.transform.translation.z = -100;
	mainCamera.transform.translation.y = 10;
}

// 更新
void TestScene::Update() {
	//mainCamera.DebugGUI();

	player_.Update();
}