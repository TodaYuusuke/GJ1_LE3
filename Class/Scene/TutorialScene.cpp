#include "TutorialScene.h"
#include"GameScene.h"

using namespace LWP;
using namespace LWP::Input;
//using namespace LWP::Primitive;
using namespace LWP::Math;
//using namespace LWP::Utility;
using namespace LWP::Object;

float LerpX(const float start, const float end, float t) {
	// 線形補間した値を返す
	return(start * (1.0f - t) + end * t);
}

void TutorialScene::Initialize()
{
	// パーティクル初期化
	particleManager_.Init();
	player_.Initialize(&particleManager_);
	followCamera_.Initialize(&mainCamera);
	enemyManager_.Initialize(&player_);
	drone_.Initialize(&player_,&enemyManager_);
	stage_.Init(&mainCamera);

	//タスクを追加
	for (int i = 0; i < _countTasks; i++) {
		taskDatas_[taskName_[i]] = false;
	}

	bgm_.Load(audioPath_ + bgmPath_);
	bgm_.Play(0.2f, 255);

	spriteBoad_.isUI = true;
	spriteBoad_.material.enableLighting = false;
	spriteBoad_.material.texture = LWP::Resource::LoadTexture("tutorial/boad.png");
	spriteBoad_.worldTF.translation = {960,220,0};
	spriteBoad_.anchorPoint = { 0.5f,0.5f };


	spriteGage_.isUI = true;
	spriteGage_.material.enableLighting = false;
	spriteGage_.material.texture = LWP::Resource::LoadTexture("tutorial/gage.png");
	spriteGage_.worldTF.translation = { 760,440,0 };
	spriteGage_.worldTF.scale = { 2.0f,1.0f,1.0f };
	spriteGage_.anchorPoint = { 0.0f,0.5f };


	//testSprite_.isUI = true;
	//testSprite_.material.enableLighting = false;
	//testSprite_.material.texture = LWP::Resource::LoadTexture("");
	//testSprite_.size;
}

void TutorialScene::Update()
{

	Debug();

	//状態リクエストがある時実行
	if (taskReq_) {
		//状態を更新
		tasks_ = taskReq_.value();
		taskReq_ = std::nullopt;
		//初期化処理
		(this->*TaskInitialize[(int)tasks_])();
		spriteGage_.worldTF.scale = { 2.0f,1.0f,1.0f };
	}
	//状態更新処理
	(this->*TaskUpdate[tasks_])();

	drone_.Update();
	player_.Update();
	followCamera_.Update(player_.GetWorldPosition());

	SceneChange();
}

void TutorialScene::Debug()
{

	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Tutorial")) {

			ImGui::Text(" now task : %s", taskName_[tasks_].c_str());

			switch (tasks_)
			{
			case TutorialScene::Move:

				ImGui::Text("move count : %4.1f", normas_.move.currentMoving);
				ImGui::DragFloat("move need move sec",&normas_.move.needMovingSec);
				break;
			case TutorialScene::Slide:
				ImGui::Text("slide isCount : %d / count : %d", normas_.slide.isCount, normas_.slide.currentCount);
				ImGui::DragInt("max count", &normas_.slide.maxCount);
				break;
			case TutorialScene::Shot:
				ImGui::Text("shot isCount : %d / count : %d", normas_.shot.isCount, normas_.shot.currentCount);
				ImGui::DragInt("max count", &normas_.shot.maxCount);
				break;
			case TutorialScene::SlideShot:
				ImGui::Text("slideshot isCount : %d / count : %d", normas_.slideShot.isCount, normas_.slideShot.currentCount);
				ImGui::DragInt("max count", &normas_.slideShot.maxCount);
				break;
			case TutorialScene::Another:
				ImGui::Text("ano count : %4.1f", normas_.another.currentMoving);
				ImGui::DragFloat("ano need move sec", &normas_.another.needMovingSec);
				break;
			case TutorialScene::_countTasks:

				break;
			default:
				break;
			}
			if (ImGui::TreeNode("boad")) {
				spriteBoad_.DebugGUI();

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("gage")) {
				spriteGage_.DebugGUI();

				ImGui::TreePop();
			}


		}

		ImGui::EndTabBar();
	}
	ImGui::End();

}

void TutorialScene::SceneChange()
{

	if (isSceneChange_) {
		nextSceneFunction = []() { return new GameScene(); };
	}

#ifdef DEMO
	// Nキーで次のシーンへ
	if (Keyboard::GetTrigger(DIK_N)) {
		nextSceneFunction = []() { return new GameScene(); };
	}
#endif // DEMO


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

	//移動状態の時カウント増やす
	float delta = Info::GetDeltaTimeF();

	if (player_.GetAnimationType() == "01_Run") {
		normas_.move.currentMoving += delta;

	}

	float t = (normas_.move.currentMoving / normas_.move.needMovingSec);
	float scale = LerpX(2, 0, t);
	spriteGage_.worldTF.scale.x = scale;

	if (normas_.move.currentMoving >= normas_.move.needMovingSec) {
		taskDatas_[taskName_[Move]] = true;
		spriteGage_.worldTF.scale.x = 0;
		//リクエストを次に
		taskReq_ = Slide;
	}

}

void TutorialScene::SlideUpdate()
{
	//任意処理を行っていないとき
	if (player_.GetAnimationType() != "02_SlidingStart") {
		//次の任意入力で反応するように
		normas_.slide.isCount = true;
	}
	//任意入力があった時
	if (normas_.slide.isCount && player_.GetAnimationType() == "02_SlidingStart") {
		normas_.slide.currentCount++;
		normas_.slide.isCount = false;
	}

	float t = (float) (normas_.slide.currentCount / (float)normas_.slide.maxCount);
	float scale = LerpX(2, 0, t);
	spriteGage_.worldTF.scale.x = scale;

	//遷移条件
	if (normas_.slide.currentCount >= normas_.slide.maxCount) {
		taskDatas_[taskName_[Slide]] = true;
		spriteGage_.worldTF.scale.x = 0;
		//次のタスクへ
		taskReq_ = Shot;
	}

}

void TutorialScene::ShotUpdate()
{
	//任意処理を行っていないとき
	if (player_.GetAnimationType() != "05_StandShot") {
		//次の任意入力で反応するように
		normas_.shot.isCount = true;
	}
	//任意入力があった時
	if (normas_.shot.isCount && player_.GetAnimationType() == "05_StandShot") {
		normas_.shot.currentCount++;
		normas_.shot.isCount = false;
	}

	float t = (float)(normas_.shot.currentCount / (float)normas_.shot.maxCount);
	float scale = LerpX(2, 0, t);
	spriteGage_.worldTF.scale.x = scale;

	//遷移条件
	if (normas_.shot.currentCount >= normas_.shot.maxCount) {
		taskDatas_[taskName_[Shot]] = true;
		spriteGage_.worldTF.scale.x = 0;
		//次のタスクへ
		taskReq_ = SlideShot;
	}
}

void TutorialScene::SlideShotUpdate()
{
	//任意処理を行っていないとき
	if (player_.GetAnimationType() != "06_SlidingShot") {
		//次の任意入力で反応するように
		normas_.slideShot.isCount = true;
	}
	//任意入力があった時
	if (normas_.slideShot.isCount && player_.GetAnimationType() == "06_SlidingShot") {
		normas_.slideShot.currentCount++;
		normas_.slideShot.isCount = false;
	}

	float t = (float)(normas_.slideShot.currentCount / (float)normas_.slideShot.maxCount);
	float scale = LerpX(2, 0, t);
	spriteGage_.worldTF.scale.x = scale;

	//遷移条件
	if (normas_.slideShot.currentCount >= normas_.slideShot.maxCount) {
		taskDatas_[taskName_[SlideShot]] = true;
		spriteGage_.worldTF.scale.x = 0;
		//次のタスクへ
		taskReq_ = Another;
	}
}

void TutorialScene::AnotherUpdate()
{
	//移動状態の時カウント増やす
	float delta = Info::GetDeltaTimeF();

	//if (player_.GetAnimationType() == "01_Run") {
		normas_.another.currentMoving += delta;

	//}

		float t =  (normas_.another.currentMoving / normas_.another.needMovingSec);
		float scale = LerpX(2, 0, t);
		spriteGage_.worldTF.scale.x = scale;

	if (normas_.another.currentMoving >= normas_.another.needMovingSec) {
		taskDatas_[taskName_[Another]] = true;
		spriteGage_.worldTF.scale.x = 0;
		//最後なので
		isSceneChange_ =true;
		
	}
}






#pragma endregion
