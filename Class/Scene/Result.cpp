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

	// ポストプロセス調整
	mainCamera.pp.grayScale.intensity = 0.0f;
	mainCamera.pp.grayScale.use = true;
	mainCamera.pp.radialBlur.center = { 0.61f, 0.33f };
	mainCamera.pp.radialBlur.blurWidth = 0.0f;
	mainCamera.pp.radialBlur.use = true;
	stage_.Init(&mainCamera);
	// カメラの位置調整
	mainCamera.transform.translation = { -31.1f, 0.39f, 1.11f };
	mainCamera.transform.rotation = { -0.053f, -0.29f, -0.016f, 0.955f };

	// プレイヤー読み込み
	player_.model.LoadShortPath("Robot/Player_Boned_IK.gltf");
	player_.anim.LoadFullPath("resources/model/Robot/Player_Boned_IK.gltf", &player_.model);
	player_.anim.Play("15_Result");
	player_.model.worldTF.translation = { -34.53f, 0.06f, 4.63f };

	// ドローン読み込み
	drone_.model.LoadShortPath("Drone/Drone.gltf");
	drone_.anim.LoadFullPath("resources/model/Drone/Drone.gltf", &drone_.model);
	drone_.anim.Play("01_Result");
	drone_.model.worldTF.translation = { -34.68f, 0.72f, 4.86f };
	droneLight_.transform.Parent(&drone_.model.worldTF);
	droneLight_.transform.translation.y = -0.72f;
	droneLight_.intensity = 0.5f;
	droneLight_.radius = 10.0f;
	droneLight_.color = { 202,255,208,255 };
}

// 更新
void Result::Update() {
	// アニメーションの再生が終わったら
	if (!ppParameter.flag && player_.anim.GetProgress() > 0.7f && drone_.anim.GetProgress() > 0.7f) {
		ppParameter.flag = true;
		mainCamera.pp.grayScale.intensity = 0.0f;
		mainCamera.pp.radialBlur.blurWidth = 0.0f;
	}
	else if (ppParameter.flag) {
		// 時間更新
		ppParameter.time += Info::GetDeltaTimeF();
		if (ppParameter.time > ppParameter.totalTime) { ppParameter.time = ppParameter.totalTime; }

		mainCamera.pp.grayScale.intensity = ResultLerp(0.0f, ppParameter.grayScaleIntensity, ppParameter.time / ppParameter.totalTime);
		mainCamera.pp.radialBlur.blurWidth = ResultLerp(0.0f, ppParameter.radialBlurIntensity, ppParameter.time / ppParameter.totalTime);
	}

	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		bgm_.Stop();
		nextSceneFunction = []() { return new NullScene([]() { return new Title(); }); };
	}
	// Rキーでもう一度
	if (Keyboard::GetTrigger(DIK_R)) {
		bgm_.Stop();
		nextSceneFunction = []() { return new NullScene([]() { return new Result(); }); };
	}
}