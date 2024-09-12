#pragma once
#include <Adapter.h>
#include "scene/IScene.h"

#include "../Stage/Stage.h"

class Result final
	: public IScene {
public:
	Result() = default;
	~Result() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//

	//オーディオがあるフォルダまでのパス
	std::string audioPath_ = "BGM/";
	//bgm
	LWP::Resource::Audio bgm_;
	std::string bgmPath_ = "clear1.mp3";

	// ステージ
	Stage stage_;
	// プレイヤー
};