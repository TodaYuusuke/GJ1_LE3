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

int Lerp(const int start, const int end, float t) {
	// 線形補間した値を返す
	return(static_cast<float>(start) * (1.0f - t) + static_cast<float>(end) * t);
}

Result::Result(bool winFlag) {
	winFlag_ = winFlag;

	// winFlag が falseならば
	if (!winFlag) {
		// プレイヤーのみ非表示に
		player_.model.isActive = false;
		drone_.model.worldTF.translation = { -34.68f, 0.42f, 4.86f };	// 少し下にする
		mainCamera.pp.radialBlur.center = { 0.69f, 0.15f };
	}
	// winFlag が trueならば勝利したので通常通り
	else {
		drone_.model.worldTF.translation = { -34.68f, 0.72f, 4.86f };
		mainCamera.pp.radialBlur.center = { 0.61f, 0.33f };
	}
}

// 初期化
void Result::Initialize() {
	//	音関係の初期化と発生
	bgm_.Load(audioPath_ + bgmPath_);
	bgm_.Play(0.1f, 255);

	// ポストプロセス調整
	mainCamera.pp.grayScale.intensity = 0.0f;
	mainCamera.pp.grayScale.use = true;
	mainCamera.pp.radialBlur.blurWidth = 0.0f;
	mainCamera.pp.radialBlur.use = true;
	stage_.Init(&mainCamera);
	// カメラの位置調整
	mainCamera.transform.translation = { -31.1f, 0.39f, 1.11f };
	mainCamera.transform.rotation = { -0.053f, -0.29f, -0.016f, 0.955f };

	// プレイヤー読み込み
	player_.model.LoadShortPath("Robot/Player_Boned_IK.gltf");
	player_.model.materials["HeadDisplay"].enableLighting = false;
	player_.anim.LoadFullPath("resources/model/Robot/Player_Boned_IK.gltf", &player_.model);
	player_.anim.Play("15_Result");
	player_.model.worldTF.translation = { -34.53f, 0.06f, 4.63f };

	// ドローン読み込み
	drone_.model.LoadShortPath("Drone/Drone.gltf");
	drone_.model.materials["Gage"].enableLighting = false;
	if (winFlag_) {
		drone_.model.materials["Gage"].uvTransform.translation.y = -0.5f;
	}
	else {
		drone_.model.materials["Gage"].texture = LWP::Resource::LoadTextureLongPath("resources/system/texture/white.png");
		drone_.model.materials["Gage"].color = Utility::ColorPattern::RED;
	}
	drone_.anim.LoadFullPath("resources/model/Drone/Drone.gltf", &drone_.model);
	drone_.anim.Play("01_Result");
	droneLight_.transform.Parent(&drone_.model.worldTF);
	droneLight_.transform.translation.y = -0.72f;
	droneLight_.intensity = 0.5f;
	droneLight_.radius = 10.0f;
	droneLight_.color = { 202,255,208,255 };

	// ゲーム結果
	resultUI_.worldTF.translation = { 100.0f, 250.0f, 0.0f };
	resultUI_.worldTF.rotation.z = -0.15f;
	if (winFlag_) {
		resultUI_.material.texture = LWP::Resource::LoadTexture("UI/Text/GAMECLEAR.png");
	}
	else {
		resultUI_.material.texture = LWP::Resource::LoadTexture("UI/Text/GAMEOVER.png");
	}
	resultUI_.material.color = { 1.0f, 1.0f, 1.0f, 0.0f };
	resultUI_.material.enableLighting = false;
	resultUI_.isUI = true;
	// ボタン
	buttonUI_.worldTF.translation = { 100.0f, 850.0f, 0.0f };
	buttonUI_.material.texture = LWP::Resource::LoadTexture("UI/Text/NextScreen.png");
	buttonUI_.material.color = { 1.0f, 1.0f, 1.0f, 0.0f };
	buttonUI_.material.enableLighting = false;
	buttonUI_.isUI = true;

	systemSE_.Load(sysPath_);

	fade_.Init();
}

// 更新
void Result::Update() {
	stage_.Update();

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

		// 結果スプライトを徐々に表示
		resultUI_.material.color.A = Lerp(resultUI_.material.color.A, 255, 0.01f);
		buttonUI_.material.color.A = Lerp(buttonUI_.material.color.A, 255, 0.01f);
	}

	// Space または Aボタンで次のシーンへ（）
	if (!fade_.GetStartOut() && (Keyboard::GetTrigger(DIK_SPACE) || Controller::GetTrigger(XBOX_A))) {
		fade_.Out();
		systemSE_.Play();
	}

	fade_.Update();
	if (fade_.GetOut()) {
		bgm_.Stop();
		nextSceneFunction = []() { return new NullScene([]() { return new Title(); }); };
	}
}