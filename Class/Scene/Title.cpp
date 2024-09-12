#include "Title.h"
#include "GameScene.h"
#include "NullScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;

// 初期化
void Title::Initialize() {
	mainCamera.transform.translation = { -42.89f, 3.16f, 3.68f };
	mainCamera.transform.rotation = { 0.079f, 0.614f, -0.062f, 0.783f };
	stage_.Init(&mainCamera);
	
	player_.model.LoadShortPath("Robot/Player_Boned_IK.gltf");
	player_.anim.LoadFullPath("resources/model/Robot/Player_Boned_IK.gltf", &player_.model);
	player_.anim.Play("14_Title", true);
	player_.model.worldTF.translation = { -34.0f, 0.0f, 7.0f };
	player_.model.worldTF.rotation = { 0.0f, 0.948f, 0.0f, -0.319f };
	
	drone_.model.LoadShortPath("Drone/Drone.gltf");
	drone_.anim.LoadFullPath("resources/model/Drone/Drone.gltf", &drone_.model);
	drone_.anim.Play("00_Idle", true);
	drone_.model.worldTF.translation = { -33.0f, 3.0f, 7.6f };
	droneLight_.transform.Parent(&drone_.model.worldTF);
	droneLight_.transform.translation.y = -0.72f;
	droneLight_.intensity = 0.5f;
	droneLight_.radius = 10.0f;
	droneLight_.color = { 202,255,208,255 };
	
	for (int i = 0; i < 3; i++) {
		spider_[i].model.LoadShortPath("Enemy/SpiderEnemy/SpiderEnemy.gltf");
		spider_[i].anim.LoadFullPath("resources/model/Enemy/SpiderEnemy/SpiderEnemy.gltf", &spider_[i].model);
	}
	// 座標設定
	spider_[0].model.worldTF.translation = { 56.75f,0.0f,0.0f };
	spider_[0].model.worldTF.rotation = { 0.0f, 0.125f, 0.0f, 0.992f };
	spider_[1].model.worldTF.translation = { 39.57f,0.0f,3.49f };
	spider_[1].model.worldTF.rotation = { 0.0f, 0.926f, 0.0f, 0.376f };
	spider_[2].model.worldTF.translation = { 34.53f,0.0f,0.0f };
	spider_[2].model.worldTF.rotation = { 0.0f, 0.748f, 0.0f, 0.663f };
	// ちょっとアニメーションをずらす
	spider_[0].anim.Play("00_Idle", true);
	spider_[1].anim.Play("00_Idle", true, 0.4f);
	spider_[2].anim.Play("00_Idle", true, 0.7f);

	// タイトル
	sprite_.worldTF.translation = { 926.0f, 116.0f, 0.0f };
	sprite_.material.texture = LWP::Resource::LoadTexture("UI/TitleLogo.png");
	sprite_.material.enableLighting = false;
	sprite_.isUI = true;
}

// 更新
void Title::Update() {
	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		nextSceneFunction = []() { return new NullScene([]() { return new GameScene(); }); };
	}
}