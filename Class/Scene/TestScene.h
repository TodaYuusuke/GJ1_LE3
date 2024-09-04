#pragma once
#include <Adapter.h>
#include "scene/IScene.h"

#include"../Player/Player.h"


class TestScene final
	: public IScene {
public:
	TestScene() = default;
	~TestScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//
	Player player_;
	//LWP::Resource::RigidModel model_;
};