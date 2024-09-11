#include "TestScene.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

// 初期化
void TestScene::Initialize() {
	player_.Initialize();

	enemyManager_.Initialize(&player_);

	// ゲームUIマネージャーの初期化
	gameUIManager_.Initialize(&player_);

	followCamera_.Initialize(&mainCamera);
	drone_.Initialize(&player_, &enemyManager_);

	// アップグレードマネージャーの初期化
	upgradeManger_.Initialize(&player_, &drone_);

	stage_.LoadShortPath("Stage/Stage.gltf");
	stage_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);
	sun_.rotation.x = 3.14;	// 下から照らす
	sun_.color = { 36,42,52,255 };
}

// 更新
void TestScene::Update() {
	player_.Update();
	//skilltree_.Update();
	followCamera_.Update(player_.GetWorldPosition());
	enemyManager_.Update();
	drone_.Update();
	// UIの更新
	gameUIManager_.Update();

	// アップグレードマネージャーの更新
	upgradeManger_.Update();
}