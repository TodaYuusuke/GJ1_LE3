#include "TestScene.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

// 初期化
void TestScene::Initialize() {
	player_.Initialize();
	//skilltree_.Initialize();

	enemyManager_.Initialize(&player_);

	followCamera_.Initialize(&mainCamera);
	drone_.Initialize(&player_, &enemyManager_);


	stage_.LoadShortPath("Stage/Stage.gltf");
	stage_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);
	sun_.rotation.x = 3.14;	// 下から照らす
}

// 更新
void TestScene::Update() {
	player_.Update();
	//skilltree_.Update();
	followCamera_.Update(player_.GetWorldPosition());
	enemyManager_.Update();
	drone_.Update();
}