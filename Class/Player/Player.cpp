#include "Player.h"
#include<numbers>
using namespace LWP;
using namespace LWP::Object;

float Lerp(const float start, const float end, float t) {
	// 線形補間した値を返す
	return(start * (1.0f - t) + end * t);
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
		if (isHitOnDebug_&&parameters_.hitData.isHit_) {
			parameters_.hitData.isHit_ = false;
			parameters_.hp--;
			parameters_.hitData.hitDirection_ = (model_.worldTF.GetWorldPosition() - data->GetWorldPosition()).x;
			parameters_.bulletData.currentPutBulletInSec_ = 0;
			parameters_.bulletData.currentReloadStartSec_ = 0;
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
	parameters_.bulletData = BulletData{};
	parameters_.jumpData = JumpData{};
	parameters_.slideData = SlidingData{};
	parameters_.hitData = HitData{};

	pVeloX_ = 1.0f;
	//最小最大角度
	float pi = (float)std::numbers::pi / 2.0f;

	model_.worldTF.rotation = Math::Quaternion::ConvertEuler({ 0,pi ,0 });

	parameters_.jumpData.isJump_ = false;

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
	if (parameters_.jumpData.isJump_) {
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

		if (parameters_.jumpData.isJump_) {
			velo_ = { 0,0,0 };
			acce_ = { 0,0,0 };
			parameters_.jumpData.isJump_ = false;
		}
	}


	//回転処理
	if (parameters_.isturn) {
		parameters_.currentTurnSec += delta;
		float t = parameters_.currentTurnSec / parameters_.turnSec;
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

			parameters_.isturn = false;
		}
	}

	ReloadBullet(delta);

	bullets_->Update();
}

void Player::ReloadBullet(float delta)
{
	//弾が最大数以下で処理を行う
	if (parameters_.bulletData.ammoRemaining_ < parameters_.bulletData.maxAmmoNum_) {
		//リロード処理開始までの待機カウント
		parameters_.bulletData.currentReloadStartSec_ += delta;
		//待機時間満了で処理
		if (parameters_.bulletData.currentReloadStartSec_ >= parameters_.bulletData.reloadStartSec_) {
			parameters_.bulletData.currentReloadStartSec_ = parameters_.bulletData.reloadStartSec_;

			//加算処理
			parameters_.bulletData.currentPutBulletInSec_ += delta;

			//時間超えで弾増加
			if (parameters_.bulletData.currentPutBulletInSec_ >= parameters_.bulletData.putBulletInSec_) {
				parameters_.bulletData.currentPutBulletInSec_ = 0;

				//弾増加
				parameters_.bulletData.ammoRemaining_ += parameters_.bulletData.simultaneouslyLoaded_;

				//最大量を超えたら修正
				if (parameters_.bulletData.ammoRemaining_ > parameters_.bulletData.maxAmmoNum_) {
					parameters_.bulletData.maxAmmoNum_;
				}
			}
		}
	}
	else {
		//最大数なのでカウント初期化
		parameters_.bulletData.currentReloadStartSec_ = 0;
		parameters_.bulletData.currentPutBulletInSec_ = 0;
	}
}



void Player::ShotBullet(const LWP::Math::Vector3& v, const std::string& cName, float shotNum)
{

	float theta = -parameters_.bulletData.bulletDispersion_ / 2;

	if (parameters_.bulletData.ammoRemaining_ <= 0) {
		return;
	}

	//弾減らす処理
	parameters_.bulletData.ammoRemaining_--;
	parameters_.bulletData.currentReloadStartSec_ = 0;
	parameters_.bulletData.currentPutBulletInSec_ = 0;

	for (int i = 0; i < shotNum; i++) {

			LWP::Math::Vector3 ve = rotateZ(v, theta);

			//一発
			bullets_->SetData(model_.worldTF.translation, { ve * parameters_.bulletData.bulletsSpd_ }, cName);

			theta += parameters_.bulletData.bulletDispersion_ / (shotNum - 1.0f);


		
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

			ImGui::Text(behaName.c_str(), parameters_.hp);
			ImGui::Checkbox("is Hit", &isHitOnDebug_);
			ImGui::DragFloat("move spd", &parameters_.moveSpd, 0.01f);
			ImGui::DragFloat("turn sec", &parameters_.turnSec, 0.01f);

			if (ImGui::TreeNode("slide")) {
				ImGui::DragFloat("slide leng", &parameters_.slideData.length, 0.01f);
				ImGui::DragFloat("slide spd", &parameters_.slideData.spd, 0.01f);
				ImGui::DragFloat("acceSlide spd", &parameters_.slideData.acceSpd, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("jump")) {
				ImGui::DragFloat("gravity", &parameters_.gravity, 0.01f);
				ImGui::DragFloat("Jump slope", &parameters_.jumpData.jumpSlope_, 0.01f);
				ImGui::DragFloat("jump Velo", &parameters_.jumpData.jumpVelo_, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("bullet")) {

				ImGui::Text("bullet remaining : %d", parameters_.bulletData.ammoRemaining_);
				ImGui::DragInt("pellet count", &parameters_.bulletData.shotpelletNum_);
				ImGui::DragInt("max ammo count", &parameters_.bulletData.maxAmmoNum_);
				ImGui::DragInt("reload bullet multiply ",&parameters_.bulletData.simultaneouslyLoaded_);

				ImGui::DragFloat("reload start sec", &parameters_.bulletData.reloadStartSec_,0.01f);
				ImGui::DragFloat("reload ammo sec", &parameters_.bulletData.putBulletInSec_, 0.01f);

				ImGui::DragFloat("bullet dispersion", &parameters_.bulletData.bulletDispersion_, 0.01f);
				
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
	parameters_.slideData.startPos = model_.worldTF.GetWorldPosition();

	//ベクトルをスライディング初速度で設定
	velo_ = Math::Vector3(pVeloX_, 0, 0).Normalize() * parameters_.slideData.spd;

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
	velo.y = parameters_.jumpData.jumpSlope_;
	velo = velo.Normalize();
	//if (preBehavior_ == Sliding) {
	//	//ジャンプの向きベクトル計算
	//	velo_.y = Math::Vector3{ velo * jumpVelo_ }.y;
	//}
	//else {
	//	velo_ = velo * jumpVelo_;
	//}

	velo_ = velo * parameters_.jumpData.jumpVelo_;
	acce_.y = -parameters_.gravity;

	parameters_.jumpData.isJump_ = true;

	SetAnimation(A_Idle);
	//ShotBullet({ 0,-1,0 });
}
void Player::InitializeHitSomeone()
{
	Math::Vector3 ve = Math::Vector3{ parameters_.hitData.hitDirection_,0,0 }.Normalize();
	ve.y = parameters_.hitData.hitHeight_;

	velo_ = ve * parameters_.hitData.hitVelocity_;
	acce_.y = -parameters_.gravity;

	//多分上に吹っ飛ぶので一応
	parameters_.jumpData.isJump_ = true;

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
		move.x -= parameters_.moveSpd;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_D)) {
		move.x += parameters_.moveSpd;
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
		parameters_.isturn = true;
		parameters_.currentTurnSec = 0;
	}




#pragma endregion


#pragma region 各状態変化
	//スライディングに移行

	if (!parameters_.jumpData.isJump_) {

		if (Input::Keyboard::GetTrigger(DIK_LSHIFT)) {
			behaviorReq_ = Sliding;
		}

		if (Input::Keyboard::GetTrigger(DIK_C)) {
			ShotBullet(Math::Vector3{ pVeloX_,0,0 }.Normalize(), standShot, (float)parameters_.bulletData.shotpelletNum_);
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
	float pLeng = (parameters_.slideData.startPos - model_.worldTF.GetWorldPosition()).Length();

	//指定した距離より離れた場合に減速処理
	if (parameters_.slideData.length < pLeng) {
		behaviorReq_ = QuitSlide;
	}

//スライディング中のアニメーション切り替え
	if (animeName_[A_SlidingStart] == nowPlayAnimeName_ && !animation.GetPlaying()) {
		SetAnimation(A_Sliding);
	}


	//スライド中に攻撃
	if (Input::Keyboard::GetTrigger(DIK_C)) {
		ShotBullet({ 0,1,0 }, slideShot, (float)parameters_.bulletData.shotpelletNum_);
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
		velo_.x -= parameters_.slideData.acceSpd * delta;

		//Pの向きが正の向き＆
		//ベクトルが負の向きの時終了
		if (velo_.x < 0) {
			behaviorReq_ = Moving;
			velo_.x = 0;
		}
	}
	else {
		//xの量引く
		velo_.x += parameters_.slideData.acceSpd * delta;

		//Pの向きが負の向き＆
		//ベクトルが正の向きの時終了
		if (velo_.x > 0) {
			behaviorReq_ = Moving;
			velo_.x = 0;
		}
	}

	//アニメーション進行度(0-1)
	float at = 1.0f - velo_.Length() / parameters_.slideData.spd;
	at;


}



void Player::UpdateJump()
{
	if (!parameters_.jumpData.isJump_) {
		behaviorReq_ = Moving;
	}
}

void Player::UpdateHitSomeone()
{
	float delta = Info::GetDeltaTimeF();
	parameters_.hitData.currentNoHit_ += delta;

	if (parameters_.hitData.currentNoHit_ >= parameters_.hitData.noHitSec_) {
		parameters_.hitData.currentNoHit_ = 0;
		parameters_.hitData.isHit_ = true;
		behaviorReq_ = Moving;
	}
}




#pragma endregion


