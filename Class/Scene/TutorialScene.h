#pragma once
#include <Adapter.h>
#include "scene/IScene.h"
#include"../Player/Player.h"
#include"../FollowCamera/FollowCamera.h"
#include"../Drone/Drone.h"

class TutorialScene final
	: public IScene {
public:
	TutorialScene() = default;
	~TutorialScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//

	static void (TutorialScene::* TaskInitialize[])();
	static void (TutorialScene::* TaskUpdate[])();

	void MoveInitialize();
	void SlideInitialize();
	void ShotInitialize();
	void SlideShotInitialize();
	void AnotherInitialize();

	void MoveUpdate();
	void SlideUpdate();
	void ShotUpdate();
	void SlideShotUpdate();
	void AnotherUpdate();

private://パラメータ
	struct MoveTask {
		float needMovingSec = 3.0f;
		float currentMoving = 0.0f;
	};
	struct SlideTask
	{
		int maxCount = 5;
		int currentCount = 0;
		bool isCount = true;
	};
	struct ShotTask
	{
		int maxCount = 10;
		int currentCount = 0;
		bool isCount = true;
	};
	struct SlideShotTask
	{
		int maxCount = 5;
		int currentCount = 0;
		bool isCount = true;
	};
	struct AnotherTask {
		float needMovingSec = 3.0f;
		float currentMoving = 0.0f;
	};

private://変数
	FollowCamera followCamera_;
	Player player_;
	Drone drone_;


	//タスク表
	enum Tasks {
		Move,
		Slide,
		Shot,
		SlideShot,
		Another,
		_countTasks
	}tasks_;
	std::optional<Tasks>taskReq_ = std::nullopt;

	//プレイヤータスク
	std::string taskName_[_countTasks] = {
		"Move",
		"Slide",
		"Shot,",
		"SlideShot",
		"anotherText"
	};
	std::vector<std::map<std::string, bool>>taskDatas_;



	//オーディオがあるフォルダまでのパス
	std::string audioPath_ = "BGM/";
	//bgm
	LWP::Resource::Audio bgm_;
	std::string bgmPath_ = "game2.mp3";

	LWP::Primitive::Sprite testSprite_;
};