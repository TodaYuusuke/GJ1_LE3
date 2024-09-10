#include "Player.h"
#include<numbers>
using namespace LWP;
using namespace LWP::Object;

float Lerp(const float start, const float end, float t) {
	// 線形補間した値を返す
	return(start * (1.0f - t) + end * t);
}

Player::Player()
{
	//model_.LoadCube();
	model_.LoadShortPath("Robot/Player_Boned_IK.gltf");
	animation.LoadFullPath("resources/model/Robot/Player_Boned_IK.gltf", &model_);
	SetAnimation(A_Idle);
	bullets_ = std::make_unique<PlayerBullets>();

	collider_.SetBroadShape(Collider::AABB());
	collider_.SetFollowTarget(&model_.worldTF);
	collider_.name = "player";

	//被弾処理
	collider_.enterLambda = [=](Collider::Collider* data) {

		//ヒットフラグが有効の時
		if (isHitOnDebug_&&isHit_) {
			isHit_ = false;
			hp_--;
			hitDirection_ = (model_.worldTF.GetWorldPosition() - data->GetWorldPosition()).x;
			bulletData_.currentPutBulletInSec_ = 0;
			bulletData_.currentReloadStartSec_ = 0;
			behaviorReq_ = HitSomeone;
		}

		};

}

Player::~Player()
{
}

void Player::Initialize()
{

	model_.worldTF.translation = { 0,0,0 };


	velo_ = { 0,0,0 };
	acce_ = { 0,0,0 };

	bullets_->Initialize();

	//元の値に修正
	slidingData_ = SlidingData{};
	bulletData_ = BulletData{};

	pVeloX_ = 1.0f;
	//最小最大角度
	float pi = (float)std::numbers::pi / 2.0f;

	model_.worldTF.rotation = Math::Quaternion::ConvertEuler({ 0,pi ,0 });

	isJump_ = false;

	behaviorReq_ = std::nullopt;
	//状態
	behavior_ = Moving;
	//変更前の状態
	preBehavior_ = Moving;
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

}

void Player::GlovalUpdate()
{

	float delta = Info::GetDeltaTimeF();

	//重力処理
	if (isJump_) {
		//acce_.y -= gravity_;
		//velo_.y -= gravity_;
	}


	//加算処理
	velo_ += acce_ * delta;
	model_.worldTF.translation += velo_ * delta;

	//0以下の時落下量を消す
	if (model_.worldTF.translation.y < 0) {
		model_.worldTF.translation.y = 0;
		velo_.y = 0;
		acce_.y = 0;

		if (isJump_) {
			velo_ = { 0,0,0 };
			acce_ = { 0,0,0 };
			isJump_ = false;
		}
	}


	//回転処理
	if (isturn_) {
		currentTurnSec_ += delta;
		float t = currentTurnSec_ / turnSec_;
		//最小最大角度
		float pi = (float)std::numbers::pi / 2.0f;
		float rotateY;
		if (pVeloX_ > 0) {
			rotateY = Lerp(-pi, pi, t);
		}
		else {
			rotateY = Lerp(pi, -pi, t);
		}
		model_.worldTF.rotation = Math::Quaternion::ConvertEuler({ 0,rotateY ,0 });

		//終わる時の処理
		if (t >= 1.0f) {
			t = 1.0f;
			if (pVeloX_ > 0) {
				rotateY = Lerp(-pi, pi, t);
			}
			else {
				rotateY = Lerp(pi, -pi, t);
			}
			model_.worldTF.rotation = Math::Quaternion::ConvertEuler({ 0,rotateY ,0 });

			isturn_ = false;
		}
	}

	ReloadBullet(delta);

	bullets_->Update();
}

void Player::ReloadBullet(float delta)
{
	//弾が最大数以下で処理を行う
	if (bulletData_.ammoRemaining_ < bulletData_.maxAmmoNum_) {
		//リロード処理開始までの待機カウント
		bulletData_.currentReloadStartSec_ += delta;
		//待機時間満了で処理
		if (bulletData_.currentReloadStartSec_ >= bulletData_.reloadStartSec_) {
			bulletData_.currentReloadStartSec_ = bulletData_.reloadStartSec_;

			//加算処理
			bulletData_.currentPutBulletInSec_ += delta;

			//時間超えで弾増加
			if (bulletData_.currentPutBulletInSec_ >= bulletData_.putBulletInSec_) {
				bulletData_.currentPutBulletInSec_ = 0;

				//弾増加
				bulletData_.ammoRemaining_ += bulletData_.simultaneouslyLoaded_;

				//最大量を超えたら修正
				if (bulletData_.ammoRemaining_ > bulletData_.maxAmmoNum_) {
					bulletData_.maxAmmoNum_;
				}
			}
		}
	}
	else {
		//最大数なのでカウント初期化
		bulletData_.currentReloadStartSec_ = 0;
		bulletData_.currentPutBulletInSec_ = 0;
	}
}

Math::Vector3 rotateZ(const Math::Vector3& velo, float theta) {
	Math::Matrix3x3 data;
	data.m[0][0] = std::cos(theta);
	data.m[0][1] = -std::sin(theta);
	data.m[0][2] = 0;

	data.m[1][0] = std::sin(theta);
	data.m[1][1] = std::cos(theta);
	data.m[1][2] = 0;

	data.m[2][0] = 0;
	data.m[2][1] = 0;
	data.m[2][2] = 1;

	return velo * data;
};

void Player::ShotBullet(const LWP::Math::Vector3& v, const std::string& cName, float shotNum)
{

	float theta = -bulletData_.bulletDispersion_ / 2;

	if (bulletData_.ammoRemaining_ <= 0) {
		return;
	}

	//弾減らす処理
	bulletData_.ammoRemaining_--;
	bulletData_.currentReloadStartSec_ = 0;
	bulletData_.currentPutBulletInSec_ = 0;

	for (int i = 0; i < shotNum; i++) {

			LWP::Math::Vector3 ve = rotateZ(v, theta);

			//一発
			bullets_->SetData(model_.worldTF.translation, { ve * bulletData_.bulletsSpd_ }, cName);

			theta += bulletData_.bulletDispersion_ / (shotNum - 1.0f);


		
	}
}

void Player::SetAnimation(AnimatinNameType type,bool loop )
{
	animation.Play(animeName_[type], loop);
	nowPlayAnimeName_ = animeName_[type];


}



float Player::GetPlayerDirection()
{
	if (pVeloX_ < 0) {
		return -1;
	}
	else {
		return 1;
	}
}

void Player::Debug()
{
#ifdef DEMO
	std::string behaName = "HP %d behavior : " + behaviorStirng_[behavior_];

	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Player")) {

			ImGui::Text(behaName.c_str(), hp_);
			ImGui::Checkbox("is Hit", &isHitOnDebug_);
			ImGui::DragFloat("move spd", &moveSpd_, 0.01f);
			ImGui::DragFloat("turn sec", &turnSec_, 0.01f);

			if (ImGui::TreeNode("slide")) {
				ImGui::DragFloat("slide leng", &slidingData_.length, 0.01f);
				ImGui::DragFloat("slide spd", &slidingData_.spd, 0.01f);
				ImGui::DragFloat("acceSlide spd", &slidingData_.acceSpd, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("jump")) {
				ImGui::DragFloat("gravity", &gravity_, 0.01f);
				ImGui::DragFloat("Jump slope", &jumpSlope_, 0.01f);
				ImGui::DragFloat("jump Velo", &jumpVelo_, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("bullet")) {

				ImGui::Text("bullet remaining : %d", bulletData_.ammoRemaining_);
				ImGui::DragInt("pellet count", &bulletData_.shotBulletNum_);
				ImGui::DragInt("max ammo count", &bulletData_.maxAmmoNum_);
				ImGui::DragInt("reload bullet multiply ",& bulletData_.simultaneouslyLoaded_);

				ImGui::DragFloat("reload start sec", &bulletData_.reloadStartSec_,0.01f);
				ImGui::DragFloat("reload ammo sec", &bulletData_.putBulletInSec_, 0.01f);

				ImGui::DragFloat("bullet dispersion", &bulletData_.bulletDispersion_, 0.01f);
				
				ImGui::TreePop();
			}

			model_.DebugGUI();
			if (ImGui::TreeNode("animation")) {
				animation.DebugGUI();
				ImGui::TreePop();
			}
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
	&Player::InitializeJump,
	&Player::InitializeHitSomeone
};
//更新初期化関数ポインタテーブル
void (Player::* Player::BehaviorUpdate[])() = {
	&Player::UpdateMove,
	&Player::UpdateSlide,
	&Player::UpdateQuitSlide,
	&Player::UpdateJump,
	&Player::UpdateHitSomeone
};

#pragma region 各状態の初期化
void Player::InitializeMove()
{
	//ベクトル初期化
	//velo_ = { 0,0,0 };
	SetAnimation(A_Run);

}
void Player::InitializeSlide()
{
	//スライディング開始地点を保存
	slidingData_.startPos = model_.worldTF.GetWorldPosition();

	//ベクトルをスライディング初速度で設定
	velo_ = Math::Vector3(pVeloX_, 0, 0).Normalize() * slidingData_.spd;

	SetAnimation(A_SlidingStart, false);

}

void Player::InitializeQuitSlide()
{
	SetAnimation(A_SlidingEnd, false);
}



void Player::InitializeJump()
{
	//初期加速度を渡す
	//acceGravity_ = jumpVelo_;
	Math::Vector3 velo;
	velo = Math::Vector3{ pVeloX_,0,0 }.Normalize();
	velo.y = jumpSlope_;
	velo = velo.Normalize();
	//if (preBehavior_ == Sliding) {
	//	//ジャンプの向きベクトル計算
	//	velo_.y = Math::Vector3{ velo * jumpVelo_ }.y;
	//}
	//else {
	//	velo_ = velo * jumpVelo_;
	//}

	velo_ = velo * jumpVelo_;
	acce_.y = -gravity_;

	isJump_ = true;

	SetAnimation(A_Idle);
	//ShotBullet({ 0,-1,0 });
}
void Player::InitializeHitSomeone()
{
	Math::Vector3 ve = Math::Vector3{ hitDirection_,0,0 }.Normalize();
	ve.y = hitHeight_;

	velo_ = ve * hitVelocity_;
	acce_.y = -gravity_;

	//多分上に吹っ飛ぶので一応
	isJump_ = true;

}

#pragma endregion



#pragma region 各状態の更新
void Player::UpdateMove()
{
	float delta = LWP::Info::GetDefaultDeltaTimeF();

	//プレイヤーの過去の向きを保存
	float pv = pVeloX_;

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
	model_.worldTF.translation += move * delta;

#pragma region アニメーション変更処理

	if (nowPlayAnimeName_ != animeName_[A_StandShot]) {

		if (move.x == 0 && move.y == 0 && move.z == 0) {

			if (nowPlayAnimeName_ == animeName_[A_StandShot] && !animation.GetPlaying()) {
				SetAnimation(A_Idle);
			}
			else if (nowPlayAnimeName_ != animeName_[A_Idle] && nowPlayAnimeName_ != animeName_[A_StandShot]) {
				SetAnimation(A_Idle);
			}
		}
		else {
			if (nowPlayAnimeName_ != animeName_[A_Run]) {
				SetAnimation(A_Run);
			}
		}
	}
	else {
		if (!animation.GetPlaying()) {
			SetAnimation(A_Idle);
		}
	}
#pragma endregion

#pragma region プレイヤー向きの切り替え処理
	if ((pv < 0 && pVeloX_ < 0) || (pv > 0 && pVeloX_ > 0)) {
		//特になし
	}
	else {
		isturn_ = true;
		currentTurnSec_ = 0;
	}




#pragma endregion


#pragma region 各状態変化
	//スライディングに移行

	if (!isJump_) {

		if (Input::Keyboard::GetTrigger(DIK_LSHIFT)) {
			behaviorReq_ = Sliding;
		}

		if (Input::Keyboard::GetTrigger(DIK_C)) {
			ShotBullet(Math::Vector3{ pVeloX_,0,0 }.Normalize(), standShot, (float)bulletData_.shotBulletNum_);
			SetAnimation(A_StandShot,false);
		}

		if (Input::Keyboard::GetTrigger(DIK_SPACE)) {
			behaviorReq_ = Jump;
		}

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

//スライディング中のアニメーション切り替え
	if (animeName_[A_SlidingStart] == nowPlayAnimeName_ && !animation.GetPlaying()) {
		SetAnimation(A_Sliding);
	}


	//スライド中に攻撃
	if (Input::Keyboard::GetTrigger(DIK_C)) {
		ShotBullet({ 0,1,0 }, slideShot, (float)bulletData_.shotBulletNum_);
		SetAnimation(A_SlidingShot,false);
	}
	if (Input::Keyboard::GetTrigger(DIK_SPACE)) {
		behaviorReq_ = Jump;
	}
}

void Player::UpdateQuitSlide()
{

	float delta = LWP::Info::GetDefaultDeltaTimeF();

	//向きによって減速処理
	if (pVeloX_ > 0) {
		//xの量引く
		velo_.x -= slidingData_.acceSpd * delta;

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
	float at = 1.0f - velo_.Length() / slidingData_.spd;
	at;


}



void Player::UpdateJump()
{
	if (!isJump_) {
		behaviorReq_ = Moving;
	}
}

void Player::UpdateHitSomeone()
{
	float delta = Info::GetDeltaTimeF();
	currentNoHit_ += delta;

	if (currentNoHit_ >= noHitSec_) {
		currentNoHit_ = 0;
		isHit_ = true;
		behaviorReq_ = Moving;
	}
}




#pragma endregion


