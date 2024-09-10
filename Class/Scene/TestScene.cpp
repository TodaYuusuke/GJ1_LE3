#include "TestScene.h"

using namespace LWP;
using namespace LWP::Math;

// 初期化
void TestScene::Initialize() {
	player_.Initialize();
	//skilltree_.Initialize();

	enemyManager_.Initialize(&player_);

	// ゲームUIマネージャーの初期化
	gameUIManager_.Initialize(&player_);

	followCamera_.Initialize(&mainCamera);
	stage_.LoadShortPath("Stage/Stage.gltf");
	stage_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);
}

// 更新
void TestScene::Update() {
	player_.Update();
	//skilltree_.Update();
	followCamera_.Update(player_.GetWorldPosition());
	enemyManager_.Update();

	// UIの更新
	gameUIManager_.Update();
}