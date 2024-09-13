#pragma once
#include <Adapter.h>
#include "scene/IScene.h"

#include "../Stage/Stage.h"
#include "FadeManager.h"

class Result final
	: public IScene {
public:
	Result(bool winFlag);
	~Result() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//

	// 勝利フラグ
	bool winFlag_ = false;

	//オーディオがあるフォルダまでのパス
	std::string audioPath_ = "BGM/";
	//bgm
	LWP::Resource::Audio bgm_;
	std::string bgmPath_ = "clear1.mp3";

	// ステージ
	Stage stage_;

	struct Skins {
		LWP::Resource::SkinningModel model;
		LWP::Resource::Animation anim;
	};
	// プレイヤー
	Skins player_;
	// ドローン
	Skins drone_;
	LWP::Object::PointLight droneLight_;

	// 演出用ポストプロセス
	struct PP {
		bool flag = false;	// 演出開始フラグ
		float grayScaleIntensity = 0.66f;	// グレースケールの最終強度
		float radialBlurIntensity = 0.01f;	// ラジアルブラーの最終強度

		// ポストプロセスの演出変化にかかる時間
		float totalTime = 0.5f;
		// 経過時間
		float time = 0.0f;
	}ppParameter;

	// ゲーム結果スプライト
	LWP::Primitive::Sprite resultUI_;
	LWP::Primitive::Sprite buttonUI_;

	LWP::Resource::Audio systemSE_;
	std::string sysPath_ = "SE/system_Select.mp3";

	FadeManager fade_;
};