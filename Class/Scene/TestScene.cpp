#include "TestScene.h"

using namespace LWP;
using namespace LWP::Math;

// 初期化
void TestScene::Initialize() {
	player_.Initialize();
	mainCamera.transform.translation.z = -100;
	mainCamera.transform.translation.y = 10;

	enemyManager_.Initialize(&player_);

	stage_.LoadShortPath("Stage/Stage.gltf");
	stage_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);
}

// 更新
void TestScene::Update() {
	player_.Update();
	enemyManager_.Update();
}