#pragma once
#include <Adapter.h>
#include "scene/IScene.h"

// ポイントライトのあるシーンからポイントライトのあるシーンに飛ぶとクラッシュするため回避用に何もないシーンを作成
class NullScene final
	: public IScene {
public:
	NullScene(std::function<IScene*()>);
	~NullScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override {};
	// 更新
	void Update() override {};


private: //*** これより先に必要な処理や変数を記述 ***//

	LWP::Primitive::Sprite sprite_;
};


// 仕方ないのでここでグローバルに関数宣言
static float ResultLerp(const float start, const float end, float t) {
	// 線形補間した値を返す
	return(start * (1.0f - t) + end * t);
}