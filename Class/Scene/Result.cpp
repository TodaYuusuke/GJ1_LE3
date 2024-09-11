#include "Result.h"
#include "Title.h"
#include "GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;

// 初期化
void Result::Initialize() {
}

// 更新
void Result::Update() {
	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		nextSceneFunction = []() { return new Title(); };
	}
}