#include "TestScene.h"

// 初期化
void TestScene::Initialize() {
	player_.Initialize();
	mainCamera.transform.translation.z = -50;
}

// 更新
void TestScene::Update() {
	//mainCamera.DebugGUI();

	player_.Update();
}