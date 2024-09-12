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
}

// 更新
void Result::Update() {
	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		bgm_.Stop();
		nextSceneFunction = []() { return new NullScene([]() { return new Title(); }); };
	}
}