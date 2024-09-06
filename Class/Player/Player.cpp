#include "Player.h"

using namespace LWP;
using namespace LWP::Object;

void Player::Initialize()
{
	model_.LoadCube();
	velo_ = { 0,0,0 };
	acce_ = { 0,0,0 };

	Collider::Capsule& aabb = collider_.SetBroadShape(Collider::Capsule());
	collider_.SetFollowTarget(&model_.worldTF);
	collider_.name = "player";
	

	collider_.enterLambda = [=](Collider::Collider* data) {hp_--; };

	
}

void Player::Update()
{
	Debug();

	//状態リクエストがある時実行
	if (behaviorReq_) {
		//過去の状態を保存
		preBehavior_ = behavior_;
		//状態を更新
		behavior_ = behaviorReq_.value();
		behaviorReq_ = std::nullopt;
		//初期化処理
		(this->*BehaviorInitialize[(int)behavior_])();
	}
	//状態更新処理
	(this->*BehaviorUpdate[behavior_])();

	GlovalUpdate();

	model_.DebugGUI();
}

void Player::GlovalUpdate()
{

	float delta = Info::GetDeltaTime();

	//重力処理
	if (isJump_) {
		//acce_.y -= gravity_;
		//velo_.y -= gravity_;
	}


	//加算処理
	velo_ += acce_*delta;
	model_.worldTF.translation += velo_*delta;

	//0以下の時落下量を消す
 	if (model_.worldTF.translation.y < 0) {
 		model_.worldTF.translation.y = 0;
		velo_.y = 0;
		acce_.y = 0;
		isJump_ = false;
	}

}



void Player::Debug()
{
#ifdef DEMO

	std::string behaName = "HP %d behavior : " + behaviorStirng_[behavior_];

	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Player")) {

			ImGui::Text(behaName.c_str(),hp_);
			ImGui::DragFloat("move spd", &moveSpd_, 0.01f);
			ImGui::DragFloat("slide leng", &slidingData_.length, 0.01f);
			ImGui::DragFloat("slide spd", &slidingData_.spd, 0.01f);
			ImGui::DragFloat("acceSlide spd", &slidingData_.acceSpd, 0.01f);
			ImGui::DragFloat("gravity", &gravity_, 0.01f);
			ImGui::DragFloat("jump Velo", &jumpVelo_, 0.01f);

			collider_.DebugGUI();
			ImGui::EndTabItem();
			
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
#endif // DEMO

}

//初期化関数ポンタテーブル
void (Player::* Player::BehaviorInitialize[])() = {
	&Player::InitializeMove,
	&Player::InitializeSlide,
	&Player::InitializeQuitSlide,
	&Player::InitializeShot,
	&Player::InitializeJump
};
//更新初期化関数ポインタテーブル
void (Player::* Player::BehaviorUpdate[])() = {
	&Player::UpdateMove,
	&Player::UpdateSlide,
	&Player::UpdateQuitSlide,
	&Player::UpdateShot,
	&Player::UpdateJump
};

#pragma region 各状態の初期化
void Player::InitializeMove()
{
	//ベクトル初期化
	velo_ = { 0,0,0 };
}
void Player::InitializeSlide()
{
	//スライディング開始地点を保存
	slidingData_.startPos = model_.worldTF.GetWorldPosition();

	//ベクトルをスライディング初速度で設定
	velo_ = Math::Vector3(pVeloX_, 0, 0).Normalize() * slidingData_.spd;
}

void Player::InitializeQuitSlide()
{

}

void Player::InitializeShot()
{
}

void Player::InitializeJump()
{
	//初期加速度を渡す
	//acceGravity_ = jumpVelo_;
	velo_.y += jumpVelo_;
	acce_.y = -gravity_;
	t = 0;
	isJump_ = true;
}


#pragma endregion

#pragma region 各状態の更新
void Player::UpdateMove()
{
	float delta = LWP::Info::GetDefaultDeltaTimeF();

	//入力による移動処理
	LWP::Math::Vector3 move{ 0,0,0 };
	if (LWP::Input::Keyboard::GetPress(DIK_A)) {
		move.x -= moveSpd_;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_D)) {
		move.x += moveSpd_;
	}

	//プレイヤーの向きを保存
	if (move.x != 0) {
		pVeloX_ = move.x;
	}

	//WorldTFに値追加
	model_.worldTF.translation += move*delta;


#pragma region 各状態変化
	//スライディングに移行
	if (Input::Keyboard::GetTrigger(DIK_LSHIFT)) {
		behaviorReq_ = Sliding;
	}

	if (Input::Keyboard::GetTrigger(DIK_C)) {
		behaviorReq_ = Shot;
	}

	if (Input::Keyboard::GetTrigger(DIK_SPACE)) {
		behaviorReq_ = Jump;
	}
#pragma endregion


}
void Player::UpdateSlide()
{

	//過去位置と現在（1F前)との距離を検索
	float pLeng = (slidingData_.startPos - model_.worldTF.GetWorldPosition()).Length();

	//指定した距離より離れた場合に減速処理
	if (slidingData_.length < pLeng) {
		behaviorReq_ = QuitSlide;
	}

	//加算処理
	//model_.worldTF.translation += velo_;

}

void Player::UpdateQuitSlide()
{

	float delta = LWP::Info::GetDefaultDeltaTimeF();

	//向きによって減速処理
	if (pVeloX_ > 0) {
		//xの量引く
		velo_.x -= slidingData_.acceSpd *delta;

		//Pの向きが正の向き＆
		//ベクトルが負の向きの時終了
		if (velo_.x < 0) {
			behaviorReq_ = Moving;
			velo_.x = 0;
		}
	}
	else {
		//xの量引く
		velo_.x += slidingData_.acceSpd * delta;

		//Pの向きが負の向き＆
		//ベクトルが正の向きの時終了
		if (velo_.x > 0) {
			behaviorReq_ = Moving;
			velo_.x = 0;
		}
	}

	//アニメーション進行度(0-1)
	float t = 1.0f - velo_.Length() / slidingData_.spd;
	t;

	//加算処理
	//model_.worldTF.translation += velo_;
}

void Player::UpdateShot()
{
	//硬直処理またはなにか処理を挟んだ後戻す
	behaviorReq_ = Moving;
}

void Player::UpdateJump()
{
	behaviorReq_ = Moving;
}




#pragma endregion


