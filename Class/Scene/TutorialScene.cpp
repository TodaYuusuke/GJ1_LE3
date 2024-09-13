#include "TutorialScene.h"
#include"GameScene.h"
#include "NullScene.h"

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
	mainCamera.transform.translation.y = 2.5f;
	mainCamera.transform.translation.z = -30.0f;

	// クラス初期化
	particleManager_.Init();
	player_.Initialize(&particleManager_);
	player_.parameters_.hitData.isDownHP_ = false;//あたってもHPが減らない
	drone_.model.LoadShortPath("Drone/Drone.gltf");
	drone_.model.materials["Gage"].enableLighting = false;
	drone_.anim.LoadFullPath("resources/model/Drone/Drone.gltf", &drone_.model);
	drone_.anim.Play("00_Idle", true);
	drone_.model.worldTF.translation = { -6.0f, 4.0f, 0.0f };
	droneLight_.transform.Parent(&drone_.model.worldTF);
	droneLight_.transform.translation.y = -0.72f;
	droneLight_.intensity = 0.5f;
	droneLight_.radius = 10.0f;
	droneLight_.color = { 202,255,208,255 };
	stage_.Init(&mainCamera);
	gameUIManager_.Initialize(&player_);

	//タスクを追加
	for (int i = 0; i < _countTasks; i++) {
		taskDatas_[taskName_[i]] = false;
	}

	//曲追加
	bgm_.Load(audioPath_ + bgmPath_);
	bgm_.Play(0.2f, 255);

	//画像データを先に読み込んで保存
	taskBoads_[Move] = LWP::Resource::LoadTexture("tutorial/boad1.png");
	taskBoads_[Slide] = LWP::Resource::LoadTexture("tutorial/boad2.png");
	taskBoads_[Another] = LWP::Resource::LoadTexture("tutorial/boad6.png");
	taskBoads_[Shot] = LWP::Resource::LoadTexture("tutorial/boad3.png");
	taskBoads_[SlideShot] = LWP::Resource::LoadTexture("tutorial/boad4.png");
	taskBoads_[SHAdvance] = LWP::Resource::LoadTexture("tutorial/boad5.png");



	//ボードスプライト初期化
	spriteBoad_.isUI = true;
	spriteBoad_.material.enableLighting = false;
	//spriteBoad_.material.texture = LWP::Resource::LoadTexture("tutorial/boad.png");
	spriteBoad_.worldTF.translation = { 960,220,0 };
	spriteBoad_.anchorPoint = { 0.5f,0.5f };

	//ゲージスプライト初期化
	spriteGage_.isUI = true;
	spriteGage_.material.enableLighting = false;
	spriteGage_.material.texture = LWP::Resource::LoadTexture("tutorial/gage.png");
	spriteGage_.worldTF.translation = { 1160,440,0 };
	spriteGage_.worldTF.scale = { 2.0f,1.0f,1.0f };
	spriteGage_.anchorPoint = { 1.0f,0.5f };

	spriteGageBack_.isUI = true;
	spriteGageBack_.material.enableLighting = false;
	spriteGageBack_.material.texture = LWP::Resource::LoadTexture("tutorial/gageBG.png");
	spriteGageBack_.worldTF.translation = { 1160,440,0 };
	spriteGageBack_.worldTF.scale = { 2.0f,1.0f,1.0f };
	spriteGageBack_.anchorPoint = { 1.0f,0.5f };

	skipGage_.isUI = true;
	skipGage_.material.enableLighting = false;
	skipGage_.material.texture = LWP::Resource::LoadTexture("tutorial/gageSkip.png");
	skipGage_.worldTF.translation = { 100,903,0 };
	skipGage_.worldTF.scale = { 0.0f,2.5f,1.0f };
	skipGage_.anchorPoint = { 0.0f,0.5f };

	skiptext_.isUI = true;
	skiptext_.material.enableLighting = false;
	skiptext_.material.texture = LWP::Resource::LoadTexture("UI/Text/TutorialSkip.png");
	skiptext_.worldTF.translation = { 100,900,0 };
	skiptext_.worldTF.scale = { 0.5f,0.5f,1.0f };
	skiptext_.anchorPoint = { 0.0f,0.5f };

	

	fade_.Init();


	decoy_ = std::make_unique<Decoy>(&player_, LWP::Math::Vector3{ 2,0,0 });
	decoy_->isAlive_ = false;
	decoy_->model_.isActive = false;
	decoy_->collider_.isActive = false;

	decoy2_ = std::make_unique<Decoy>(&player_, LWP::Math::Vector3{ -2,0,0 });
	decoy2_->isAlive_ = false;
	decoy2_->model_.isActive = false;
	decoy2_->collider_.isActive = false;


	systemSE_.Load(sysPath_);
}

void TutorialScene::Update() {
	stage_.Update();
	particleManager_.Update();

	//デバッグ
	Debug();

	// UIの遅延用
	if (uiInitFlag == 0) {
		gameUIManager_.SetUp();
		uiInitFlag++;
	}
	else if (uiInitFlag == -1) {
		uiInitFlag++;
	}

	//状態リクエストがある時実行
	if (taskReq_) {
		//状態を更新
		tasks_ = taskReq_.value();
		taskReq_ = std::nullopt;
		//初期化処理
		(this->*TaskInitialize[(int)tasks_])();

		//ゲージのサイズを初期化
		spriteGage_.worldTF.scale = { 2.0f,1.0f,1.0f };
	}


	//クラス更新
	player_.Update();
	player_.SetArea();
	decoy_->Update();
	decoy2_->Update();

	// 外で更新
	gameUIManager_.Update();

	//状態更新処理
	(this->*TaskUpdate[tasks_])();

	//シーン変更処理
	SceneChange();
}

void TutorialScene::Debug()
{
#if DEMO
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Tutorial")) {

			ImGui::Text(" now task : %s", taskName_[tasks_].c_str());

			switch (tasks_)
			{
			case TutorialScene::Move:

				ImGui::Text("move count : %4.1f", normas_.move.currentMoving);
				ImGui::DragFloat("move need move sec", &normas_.move.needMovingSec);
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
			if (ImGui::TreeNode("gageBack")) {
				spriteGageBack_.DebugGUI();

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("skipGage")) {
				skipGage_.DebugGUI();

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("skiptext")) {
				skiptext_.DebugGUI();

				ImGui::TreePop();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
#endif
}

void TutorialScene::SceneChange()
{

	if (!isSceneChange_) {

		float delta = Info::GetDeltaTimeF();

		//押し続けるとカウント増加
		if (Input::Keyboard::GetPress(DIK_W)|| Input::Keyboard::GetPress(DIK_UP)||Input::Controller::GetPress(XINPUT_GAMEPAD_A)) {
			currentScceneChange_ += delta;
		}
		else {
			currentScceneChange_ -= delta;
			if (currentScceneChange_ < 0) {
				currentScceneChange_ = 0;
			}
		}

		float t = currentScceneChange_ / sceneChangeSec_;
		skipGage_.worldTF.scale.x = LerpX(0.0f, 2.0f, t);

		if (currentScceneChange_ >= sceneChangeSec_) {
			isSceneChange_ = true;
			skipGage_.worldTF.scale.x = 2.0f;
			systemSE_.Play();
		}

	}

	//シーン変更フラグがONの時
	if (isSceneChange_) {
		fade_.Out();
	}

	// フェードインアウト更新
	fade_.Update();
	// フェードアウトが終わったら
	if (fade_.GetOut()) {
		// 曲を止めてシーン変更
		bgm_.Stop();
		player_.StopAllLoopSE();

		nextSceneFunction = []() { return new NullScene([]() { return new GameScene(); }); };
	}
}

void (TutorialScene::* TutorialScene::TaskInitialize[])() = {
	&TutorialScene::MoveInitialize,
	&TutorialScene::SlideInitialize,
	&TutorialScene::ShotInitialize,
	&TutorialScene::SlideShotInitialize,
	&TutorialScene::SHAdvanceInitialize,
	&TutorialScene::AnotherInitialize
};

void (TutorialScene::* TutorialScene::TaskUpdate[])() = {
	&TutorialScene::MoveUpdate,
	&TutorialScene::SlideUpdate,
	&TutorialScene::ShotUpdate,
	&TutorialScene::SlideShotUpdate,
	&TutorialScene::SHAdvanceUpdate,
	&TutorialScene::AnotherUpdate
};

#pragma region 各タスク初期化
void TutorialScene::MoveInitialize()
{
	spriteBoad_.material.texture = taskBoads_[Move];
}

void TutorialScene::SlideInitialize()
{
	spriteBoad_.material.texture = taskBoads_[Slide];
}

void TutorialScene::ShotInitialize()
{
	decoy_->isAlive_ = true;
	decoy_->model_.isActive = true;
	decoy_->collider_.isActive = true;

	spriteBoad_.material.texture = taskBoads_[Shot];
}

void TutorialScene::SlideShotInitialize()
{

	decoy_->model_.worldTF.translation = { 2,0,0 };
	decoy_->behaviorReq_ = IEnemy::Normal;
	decoy_->isAlive_ = true;
	decoy_->model_.isActive = true;
	decoy_->collider_.isActive = true;


	spriteBoad_.material.texture = taskBoads_[SlideShot];

}

void TutorialScene::SHAdvanceInitialize()
{
	decoy_->model_.worldTF.translation = { 2,0,0 };
	decoy_->behaviorReq_ = IEnemy::Normal;
	decoy_->isAlive_ = true;
	decoy_->model_.isActive = true;
	decoy_->collider_.isActive = true;

	decoy2_->model_.worldTF.translation = { -2,0,0 };
	decoy2_->behaviorReq_ = IEnemy::Normal;
	decoy2_->isAlive_ = true;
	decoy2_->model_.isActive = true;
	decoy2_->collider_.isActive = true;

	spriteBoad_.material.texture = taskBoads_[SHAdvance];
}

void TutorialScene::AnotherInitialize()
{
	spriteBoad_.material.texture = taskBoads_[Another];
}
#pragma endregion


#pragma region 各タスク更新

void TutorialScene::MoveUpdate()
{

	//移動状態の時カウント増やす
	float delta = Info::GetDeltaTimeF();

	//modelアニメーションが走るの時のみカウント
	if (player_.GetAnimationType() == "01_Run") {
		normas_.move.currentMoving += delta;

	}

	//カウント進行度でゲージのサイズ調整
	float t = (normas_.move.currentMoving / normas_.move.needMovingSec);
	float scale = LerpX(2, 0, t);
	spriteGage_.worldTF.scale.x = scale;

	//条件達成で次のタスクへ
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

	//カウント進行度でゲージのサイズ調整
	float t = (float)(normas_.slide.currentCount / (float)normas_.slide.maxCount);
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
	//if (player_.GetAnimationType() != "05_StandShot") {
	//	//次の任意入力で反応するように
	//	normas_.shot.isCount = true;
	//}

		//任意入力があった時
	if (decoy_->GetBehavior() == IEnemy::Behavior::DeadBody) {

		//再配置
		decoy_->model_.worldTF.translation = { 2,0,0 };
		decoy_->behaviorReq_ = IEnemy::Normal;
		decoy_->isAlive_ = true;
		decoy_->model_.isActive = true;
		decoy_->collider_.isActive = true;
	}


	//任意入力があった時
	if (player_.parameters_.bulletData.isShot_) {
		normas_.shot.currentCount++;
		//normas_.shot.isCount = false;
	}
	//カウント進行度でゲージのサイズ調整
	float t = (float)(normas_.shot.currentCount / (float)normas_.shot.maxCount);
	float scale = LerpX(2, 0, t);
	spriteGage_.worldTF.scale.x = scale;

	//遷移条件
	if (normas_.shot.currentCount >= normas_.shot.maxCount) {
		taskDatas_[taskName_[Shot]] = true;
		spriteGage_.worldTF.scale.x = 0;
		//次のタスクへ
		taskReq_ = Another;
	}
}

void TutorialScene::SlideShotUpdate()
{
	//範囲外で初期位置へ
	if (decoy_->model_.worldTF.translation.x < -12.0f || decoy_->model_.worldTF.translation.x > 12.0f) {
		decoy_->model_.worldTF.translation.x = 2.0f;
	}

	//任意入力があった時
	if (decoy_->GetBehavior() == IEnemy::Behavior::DeadBody) {
		normas_.slideShot.currentCount++;

		//カウントがたりない場合再配置
		if (normas_.slideShot.currentCount < normas_.slideShot.maxCount) {
			decoy_->model_.worldTF.translation = { 2,0,0 };
			decoy_->behaviorReq_ = IEnemy::Normal;
			decoy_->isAlive_ = true;
			decoy_->model_.isActive = true;
			decoy_->collider_.isActive = true;
		}
	}
	//カウント進行度でゲージのサイズ調整
	float t = (float)(normas_.slideShot.currentCount / (float)normas_.slideShot.maxCount);
	float scale = LerpX(2, 0, t);
	spriteGage_.worldTF.scale.x = scale;

	//遷移条件
	if (normas_.slideShot.currentCount >= normas_.slideShot.maxCount) {
		taskDatas_[taskName_[SlideShot]] = true;
		spriteGage_.worldTF.scale.x = 0;
		//次のタスクへ
		taskReq_ = SHAdvance;
	}
}

void TutorialScene::SHAdvanceUpdate()
{

	//スライディング開始状態取得で認識開始
	if (player_.GetAnimationType() == "02_SlidingStart" || player_.GetAnimationType() == "04_SlidingEnd" || player_.GetAnimationType() == "03_Sliding") {
		normas_.shAdvance.isCount = true;
	}
	else {

		//trueからfalseの時初期化
		if (normas_.shAdvance.isCount) {
			//再配置処理
			decoy_->model_.worldTF.translation = { 2,0,0 };
			decoy_->behaviorReq_ = IEnemy::Normal;
			decoy_->isAlive_ = true;
			decoy_->model_.isActive = true;
			decoy_->collider_.isActive = true;

			decoy2_->model_.worldTF.translation = { -2,0,0 };
			decoy2_->behaviorReq_ = IEnemy::Normal;
			decoy2_->isAlive_ = true;
			decoy2_->model_.isActive = true;
			decoy2_->collider_.isActive = true;
		}

		normas_.shAdvance.isCount = false;
	}



	//スライディング中に敵が0になったとき成功

	if (normas_.shAdvance.isCount && decoy_->GetBehavior() == IEnemy::Behavior::DeadBody && decoy2_->GetBehavior() == IEnemy::Behavior::DeadBody) {
		spriteGage_.worldTF.scale.x = 0;
		//次のタスクへ
		systemSE_.Play();
		isSceneChange_ = true;;
	}



}

void TutorialScene::AnotherUpdate()
{
	//移動状態の時カウント増やす
	float delta = Info::GetDeltaTimeF();

	//if (player_.GetAnimationType() == "01_Run") {
	normas_.another.currentMoving += delta;

	//}
	//カウント進行度でゲージのサイズ調整
	float t = (normas_.another.currentMoving / normas_.another.needMovingSec);
	float scale = LerpX(2, 0, t);
	spriteGage_.worldTF.scale.x = scale;

	if (normas_.another.currentMoving >= normas_.another.needMovingSec) {
		taskDatas_[taskName_[Another]] = true;
		spriteGage_.worldTF.scale.x = 0;

		taskReq_ = SlideShot;

	}
}






#pragma endregion
