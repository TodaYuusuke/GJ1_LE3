#include "TestScene.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

// 初期化
void TestScene::Initialize() {
	player_.Initialize();
	mainCamera.transform.translation.z = -22;
	mainCamera.transform.translation.y = 10;

	enemyManager_.Initialize(&player_);

	followCamera_.Initialize(&mainCamera);
	stage_.LoadShortPath("Stage/Stage.gltf");
	stage_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);

	drone_.Initialize(&player_, &enemyManager_);
}

// 更新
void TestScene::Update() {
	player_.Update();
	followCamera_.Update(player_.GetWorldPosition());
	enemyManager_.Update();
	drone_.Update();
}