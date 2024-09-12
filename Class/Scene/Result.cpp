#include "Result.h"
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
void Result::Initialize() {
	//	音関係の初期化と発生
	bgm_.Load(audioPath_ + bgmPath_);
	bgm_.Play(0.1f, 255);

	stage_.Init(&mainCamera);
	// カメラの位置調整
	mainCamera.transform.translation = { -30.0f, 0.5f, -1.8f };
	mainCamera.transform.rotation = { -0.039f, -0.267f, -0.011f, 0.963f };

	// プレイヤー読み込み
	player_.model.LoadShortPath("Robot/Player_Boned_IK.gltf");
	player_.anim.LoadFullPath("resources/model/Robot/Player_Boned_IK.gltf", &player_.model);
	player_.anim.Play("14_Title", true);
	player_.model.worldTF.translation = { -34.0f, 0.0f, 7.0f };
	player_.model.worldTF.rotation = { 0.0f, 0.948f, 0.0f, -0.319f };

	// ドローン読み込み
	drone_.model.LoadShortPath("Drone/Drone.gltf");
	drone_.anim.LoadFullPath("resources/model/Drone/Drone.gltf", &drone_.model);
	drone_.anim.Play("00_Idle", true);
	drone_.model.worldTF.translation = { -33.0f, 3.0f, 7.6f };
	droneLight_.transform.Parent(&drone_.model.worldTF);
	droneLight_.transform.translation.y = -0.72f;
	droneLight_.intensity = 0.5f;
	droneLight_.radius = 10.0f;
	droneLight_.color = { 202,255,208,255 };
}

// 更新
void Result::Update() {
	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		bgm_.Stop();
		nextSceneFunction = []() { return new NullScene([]() { return new Title(); }); };
	}
}