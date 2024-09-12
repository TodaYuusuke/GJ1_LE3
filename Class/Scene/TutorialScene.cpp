#include "TutorialScene.h"
#include"GameScene.h"

using namespace LWP;
using namespace LWP::Input;
//using namespace LWP::Primitive;
using namespace LWP::Math;
//using namespace LWP::Utility;
using namespace LWP::Object;

void TutorialScene::Initialize()
{

	player_.Initialize();
	followCamera_.Initialize(&mainCamera);

	//タスクを追加
	taskDatas_.reserve(5);
	for (int i = 0; i < _countTasks; i++) {
		std::map<std::string, bool> newd;
		newd[taskName_[i]]= false;
		taskDatas_.push_back(newd);
	}

	

	//testSprite_.isUI = true;
	//testSprite_.material.enableLighting = false;
	//testSprite_.material.texture = LWP::Resource::LoadTexture("");
	//testSprite_.size;
}

void TutorialScene::Update()
{

	//状態リクエストがある時実行
	if (taskReq_) {
		//状態を更新
		tasks_ = taskReq_.value();
		taskReq_ = std::nullopt;
		//初期化処理
		(this->*TaskInitialize[(int)tasks_])();
	}
	//状態更新処理
	(this->*TaskUpdate[tasks_])();

	player_.Update();
	followCamera_.Update(player_.GetWorldPosition());

	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		nextSceneFunction = []() { return new GameScene(); };
	}
}

void (TutorialScene::* TutorialScene::TaskInitialize[])() = {
	&TutorialScene::MoveInitialize,
	&TutorialScene::SlideInitialize,
	&TutorialScene::ShotInitialize,
	&TutorialScene::SlideInitialize,
	&TutorialScene::AnotherInitialize
};

void (TutorialScene::* TutorialScene::TaskUpdate[])() = {
	&TutorialScene::MoveUpdate,
	& TutorialScene::SlideUpdate,
	& TutorialScene::ShotUpdate,
	& TutorialScene::SlideShotUpdate,
	& TutorialScene::AnotherUpdate
};

#pragma region 各タスク初期化
void TutorialScene::MoveInitialize()
{
}

void TutorialScene::SlideInitialize()
{
}

void TutorialScene::ShotInitialize()
{
}

void TutorialScene::SlideShotInitialize()
{
}

void TutorialScene::AnotherInitialize()
{
}
#pragma endregion


#pragma region 各タスク更新

void TutorialScene::MoveUpdate()
{

	

}

void TutorialScene::SlideUpdate()
{
}

void TutorialScene::ShotUpdate()
{
}

void TutorialScene::SlideShotUpdate()
{
}

void TutorialScene::AnotherUpdate()
{
}


#pragma endregion
