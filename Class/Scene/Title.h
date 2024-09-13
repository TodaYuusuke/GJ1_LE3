#pragma once
#include <Adapter.h>
#include "scene/IScene.h"

#include "../Stage/Stage.h"
#include "FadeManager.h"

class Title final
	: public IScene {
public:
	Title() = default;
	~Title() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//
	struct Skins {
		LWP::Resource::SkinningModel model;
		LWP::Resource::Animation anim;
	};

	// プレイヤー
	Skins player_;
	// ドローン
	Skins drone_;
	LWP::Object::PointLight droneLight_;

	// 敵
	Skins spider_[3];

	Stage stage_;

	// タイトルロゴ
	LWP::Primitive::Sprite logo_;
	// 開始テキスト
	LWP::Primitive::Sprite buttonText_;
	// BGM
	LWP::Resource::Audio bgm_;

	// フェードインアウト
	FadeManager fade_;

	LWP::Resource::Audio systemSE_;
	std::string sysPath_ = "SE/system_Select.mp3";
};