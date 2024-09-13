#include "Title.h"
#include "TutorialScene.h"
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
	player_.model.materials["HeadDisplay"].enableLighting = false;
	player_.anim.LoadFullPath("resources/model/Robot/Player_Boned_IK.gltf", &player_.model);
	player_.anim.Play("14_Title", true);
	player_.model.worldTF.translation = { -34.0f, 0.0f, 7.0f };
	player_.model.worldTF.rotation = { 0.0f, 0.948f, 0.0f, -0.319f };
	
	drone_.model.LoadShortPath("Drone/Drone.gltf");
	drone_.model.materials["Gage"].enableLighting = false;
	drone_.anim.LoadFullPath("resources/model/Drone/Drone.gltf", &drone_.model);
	drone_.anim.Play("00_Idle", true);
	drone_.model.worldTF.translation = { -32.76f, 2.0f, 9.25f };
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
	logo_.worldTF.translation = { 926.0f, 116.0f, 0.0f };
	logo_.material.texture = LWP::Resource::LoadTexture("UI/TitleLogo.png");
	logo_.material.enableLighting = false;
	logo_.isUI = true;

	// ボタンテキスト
	buttonText_.worldTF.translation = { 926.0f, 750.0f, 0.0f };
	buttonText_.material.texture = LWP::Resource::LoadTexture("UI/Text/EnterTitle.png");
	buttonText_.material.enableLighting = false;
	buttonText_.isUI = true;

	// BGM
	bgm_.Load("BGM/title.mp3");
	bgm_.Play(0.1f, 255);

	systemSE_.Load(sysPath_);

	fade_.Init();
}

// 更新
void Title::Update() {
	stage_.Update();

	// スペースかAボタンで次のシーンへ
	if (Keyboard::GetTrigger(DIK_SPACE) || Controller::GetTrigger(XBOX_A)) {
		systemSE_.Play();
		fade_.Out();	// フェードアウト開始
	}

	// フェードアウトが終わったら次のシーンへ
	fade_.Update();
	if (fade_.GetOut()) {
		bgm_.Stop();
		nextSceneFunction = []() { return new NullScene([]() { return new TutorialScene(); }); };
	}
}