#include "Player.h"
#include "../Mask/Mask.h"
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
	model_.LoadShortPath("Robot/Player_Boned_IK.gltf");
	model_.materials["HeadDisplay"].enableLighting = false;
	animation.LoadFullPath("resources/model/Robot/Player_Boned_IK.gltf", &model_);
	SetAnimation(A_Idle);
	bullets_ = std::make_unique<PlayerBullets>();

	aabb_.collider.SetFollowTarget(&model_.worldTF);
	aabb_.collider.mask.SetBelongFrag(GJMask::Player());	// フラグ設定
	aabb_.collider.mask.SetHitFrag(GJMask::Enemy() | GJMask::HealItem());
	aabb_.collider.name = "player";

	standAABB_.min = { -0.4f,0.0f,-0.4f };
	standAABB_.max = { 0.3f,1.66f,0.5f };

	slideAABB_.min = { -0.37f,0.0f,-0.5f };
	slideAABB_.max = { 0.6f,0.55f,0.5f };

	//被弾処理
	aabb_.collider.stayLambda = [=](Collider::Collider* data) {

		if (data->name == "Spider" && (nowPlayAnimeName_ == animeName_[A_Sliding]) || data->name == standShot || data->name == slideShot) {
			return;
		}

		//回復処理
		if (data->name == "HealItem") {
			//HPの回復処理
			if (++parameters_.hp > parameters_.maxHp) {
				parameters_.hp = parameters_.maxHp;
			}
			audioGetHeal_.Play();
			return;
		}

		//ヒットフラグが有効の時
		if (isHitOnDebug_ && parameters_.hitData.isHit_) {
			
			if (parameters_.hitData.isDownHP_) {
				parameters_.hp--;
			}
			parameters_.hitData.hitDirection_ = (model_.worldTF.GetWorldPosition() - data->GetWorldPosition()).x;
			parameters_.bulletData.currentPutBulletInSec_ = 0;
			parameters_.bulletData.currentReloadStartSec_ = 0;
			behaviorReq_ = HitSomeone;

			if (parameters_.hp <= 0) {
				behaviorReq_ = Dead;
			}
			else {
				audioHit_.Play();
				parameters_.hitData.isHit_ = false;
			}

			//SetAnimation(A_Damage, false);
		}

		};



	audioRun_.Load(audioPath_ + runPath_);
	audioSlide_.Load(audioPath_ + slidePath_);
	audioGetHeal_.Load(audioPath_ + healPath_);
	audioShot_.Load(audioPath_ + shotPath_);
	audioReload_.Load(audioPath_ + reloadPath_);
	audioHit_.Load(audioPath_ + hitPath_);
}

Player::~Player()
{
}

void Player::Initialize(ParticleManager* particle)
{
	particle_ = particle;

	model_.worldTF.translation = { 0,0,0 };
	aabb_.aabb.min = standAABB_.min;
	aabb_.aabb.max = standAABB_.max;

	velo_ = { 0,0,0 };
	acce_ = { 0,0,0 };

	bullets_->Initialize(particle);

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

	parameters_.bulletData.isShot_ = false;

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

	// エリア外にいけないように
	if (model_.worldTF.translation.x < -outArea_) {
		model_.worldTF.translation.x = -outArea_;
	}
	else if (model_.worldTF.translation.x > outArea_) {
		model_.worldTF.translation.x = outArea_;
	}

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

		float min = pi;
		float max = pi * 3;

		float rotateY;
		if (pVeloX_ > 0) {
			rotateY = Lerp(max, min, t);
		}
		else {
			rotateY = Lerp(min, max, t);
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

	HitUpdate();

	bullets_->Update();
}

void Player::HitUpdate()
{
	//ヒット判定が切れているときのみ処理
	if (!parameters_.hitData.isHit_) {
		float delta = Info::GetDeltaTimeF();

		//データの加算処理
		parameters_.hitData.currentNoHit_ += delta;
		parameters_.hitData.animeCount_++;

		//点滅処理
		if (parameters_.hitData.animeCount_ % parameters_.hitData.tenmetuCount_ == 0) {
			if (model_.isActive) {
				model_.isActive = false;
			}
			else {
				model_.isActive = true;
			}
		}

		//終了処理
		if (parameters_.hitData.currentNoHit_ >= parameters_.hitData.noHitSec_) {
			parameters_.hitData.currentNoHit_ = 0;
			parameters_.hitData.animeCount_ = 0;
			parameters_.hitData.isHit_ = true;
			model_.isActive = true;
			//behaviorReq_ = Moving;
		}
	}
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
				
				audioReload_.Play();

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

bool Player::ShotBullet(const LWP::Math::Vector3& v, const std::string& cName, float shotNum)
{

	float theta = -parameters_.bulletData.bulletDispersion_ / 2;

	if (parameters_.bulletData.ammoRemaining_ <= 0) {
		return false;
	}

	audioShot_.Play();

	//弾減らす処理
	parameters_.bulletData.ammoRemaining_--;
	parameters_.bulletData.currentReloadStartSec_ = 0;
	parameters_.bulletData.currentPutBulletInSec_ = 0;

	for (int i = 0; i < shotNum; i++) {

		LWP::Math::Vector3 ve = rotateZ(v, theta);

		//一発
		bullets_->SetData(model_.worldTF.translation + parameters_.bulletData.offset_, { ve * parameters_.bulletData.bulletsSpd_ }, cName);

		theta += parameters_.bulletData.bulletDispersion_ / (shotNum - 1.0f);



	}
	parameters_.bulletData.isShot_ = true;

	return true;
}
#pragma region 各入力

void Player::ToSliding()
{
	if (Input::Keyboard::GetTrigger(DIK_LSHIFT) || Input::Pad::GetTrigger(XINPUT_GAMEPAD_B)) {
		behaviorReq_ = Sliding;
	}
}

bool Player::ToShot(const LWP::Math::Vector3& velo, const std::string& ammoName)
{
	if (Input::Keyboard::GetTrigger(DIK_SPACE) || Input::Pad::GetTrigger(XBOX_RT)) {
		bool ans = ShotBullet(velo, ammoName, (float)parameters_.bulletData.shotpelletNum_);

		if (ans) {
			//状態に
			if (ammoName == standShot) {
				SetAnimation(A_StandShot, false);
			}
			else {
				SetAnimation(A_SlidingShot, false);
			}
			return true;
		}
		else {
			return false;
		}

	}

	return false;
}

void Player::ToJump() {
	if (parameters_.activeFlag.jump && (Input::Keyboard::GetTrigger(DIK_UP) ||Input::Keyboard::GetTrigger(DIK_W) || Input::Pad::GetTrigger(XINPUT_GAMEPAD_A))) {
		//残弾がある時のみ処理
		if (parameters_.bulletData.ammoRemaining_ > 0) {
			parameters_.bulletData.ammoRemaining_--;
			behaviorReq_ = Jump;

			SetAnimation(A_JumpStart, false);
		}
	}
}
#pragma endregion


void Player::SetAnimation(AnimatinNameType type, bool loop)
{
	//歩行サウンドに関する処理
	if (nowPlayAnimeName_ == animeName_[A_Run]) {
		audioRun_.Stop();
	}
	else if (animeName_[type] == animeName_[A_Run]) {
		audioRun_.Play(audioVolume_, 255);
	}

	//スライドサウンド
	//単発系はこれでいい
	if (animeName_[type] == animeName_[A_SlidingStart]) {
		audioSlide_.Play(0.5f);
	}
	//聞こえてほしくないモーションの時削除
	if ((nowPlayAnimeName_ == animeName_[A_SlidingStart] || nowPlayAnimeName_ == animeName_[A_Sliding]) && (animeName_[type] == animeName_[A_JumpStart] || animeName_[type] == animeName_[A_Damage])) {
		audioSlide_.Stop();
	}

	animation.Play(animeName_[type], loop);
	nowPlayAnimeName_ = animeName_[type];
}

void Player::SetArea()
{
	if (model_.worldTF.translation.x < -12.0f) {
		model_.worldTF.translation.x = -12.0f;
	}
	else if (model_.worldTF.translation.x > 12.0f) {
		model_.worldTF.translation.x = 12.0f;
	}
}

void Player::StopAllLoopSE()
{
	
	audioRun_.Stop();
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

	//無限バンダナ
	static bool isEndlessAmmo_ = false;

	std::string behaName = "HP %d behavior : " + behaviorStirng_[behavior_];

	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Player")) {

			ImGui::Text(behaName.c_str(), parameters_.hp);
			ImGui::Checkbox("is Hit", &isHitOnDebug_);
			ImGui::DragFloat("move spd", &parameters_.moveSpd, 0.01f);
			ImGui::DragFloat("turn sec", &parameters_.turnSec, 0.01f);
			ImGui::Text("inertia count : %4.1f", parameters_.currentInertia);
			ImGui::DragFloat("max move sec", &parameters_.movingInertiaSec, 0.01f);
			ImGui::DragFloat("stop deceleation double", &parameters_.stopDecelerationDouble_, 0.1f);
			ImGui::DragFloat("standShot deceleation double", &parameters_.standShotDecelerationDouble_, 0.1f);

			if (ImGui::TreeNode("Active Flag")) {

				ImGui::Checkbox("jump", &parameters_.activeFlag.jump);
				ImGui::Checkbox("slidingStopShot", &parameters_.activeFlag.slidingStopShot);


				ImGui::TreePop();
			}

			if (ImGui::TreeNode("slide")) {
				ImGui::DragFloat("slide leng", &parameters_.slideData.length, 0.01f);
				ImGui::DragFloat("slide spd", &parameters_.slideData.spd, 0.01f);
				ImGui::Text("inertiaa count : %4.1f", &parameters_.currentInertia);
				ImGui::DragFloat("acceSlide spd", &parameters_.slideData.acceSpd, 0.01f);

				ImGui::DragFloat(" shot slope ", &parameters_.slideData.shotSlope, 0.01f);

				ImGui::DragFloat("reaction height", &parameters_.slideData.jumpSlope_, 0.01f);
				ImGui::DragFloat("reaction velo", &parameters_.slideData.startVelo, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("jump")) {
				ImGui::DragFloat("gravity", &parameters_.gravity, 0.01f);
				ImGui::DragFloat("Jump slope", &parameters_.jumpData.jumpSlope_, 0.01f);
				ImGui::DragFloat("jump Velo", &parameters_.jumpData.jumpVelo_, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("bullet")) {
				ImGui::Checkbox("endless ammo", &isEndlessAmmo_);
				ImGui::DragFloat3("offset pos", &parameters_.bulletData.offset_.x, 0.01f);
				ImGui::Text("bullet remaining : %d", parameters_.bulletData.ammoRemaining_);
				ImGui::DragInt("pellet count", &parameters_.bulletData.shotpelletNum_);
				ImGui::DragInt("max ammo count", &parameters_.bulletData.maxAmmoNum_);
				ImGui::DragInt("reload bullet multiply ", &parameters_.bulletData.simultaneouslyLoaded_);

				ImGui::DragFloat("reload start sec", &parameters_.bulletData.reloadStartSec_, 0.01f);
				ImGui::DragFloat("reload ammo sec", &parameters_.bulletData.putBulletInSec_, 0.01f);

				ImGui::DragFloat("bullet dispersion", &parameters_.bulletData.bulletDispersion_, 0.01f);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("hit")) {

				ImGui::Text("isHit : %d", parameters_.hitData.isHit_);
				ImGui::DragFloat("no hit sec", &parameters_.hitData.noHitSec_);
				ImGui::DragFloat("hit height", &parameters_.hitData.hitHeight_);
				ImGui::DragFloat("hit velo", &parameters_.hitData.hitVelocity_);
				ImGui::DragInt("tenmetsu count", &parameters_.hitData.tenmetuCount_);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("AABB")) {
				aabb_.aabb.DebugGUI();
				ImGui::TreePop();
			}
			model_.DebugGUI();
			if (ImGui::TreeNode("animation")) {
				animation.DebugGUI();
				ImGui::TreePop();
			}
			aabb_.collider.DebugGUI();
			ImGui::EndTabItem();

		}

		ImGui::EndTabBar();
	}
	ImGui::End();

	if (isEndlessAmmo_) {
		parameters_.bulletData.ammoRemaining_ = parameters_.bulletData.maxAmmoNum_;
	}
#endif // DEMO

}

//初期化関数ポンタテーブル
void (Player::* Player::BehaviorInitialize[])() = {
	&Player::InitializeMove,
	&Player::InitializeSlide,
	&Player::InitializeQuitSlide,
	&Player::InitializeJump,
	&Player::InitializeSlideStopShot,
	&Player::InitializeHitSomeone,
	&Player::InitializeDead
};
//更新初期化関数ポインタテーブル
void (Player::* Player::BehaviorUpdate[])() = {
	&Player::UpdateMove,
	&Player::UpdateSlide,
	&Player::UpdateQuitSlide,
	&Player::UpdateJump,
	&Player::UpdateSlideStopShot,
	&Player::UpdateHitSomeone,
	&Player::UpdateDead
};

#pragma region 各状態の初期化
void Player::InitializeMove()
{
	//ベクトル初期化
	velo_ = { 0,0,0 };
	//SetAnimation(A_Run);

	parameters_.currentInertia = 0;

	aabb_.aabb.min = standAABB_.min;
	aabb_.aabb.max = standAABB_.max;


}
void Player::InitializeSlide()
{
	//スライディング開始地点を保存
	parameters_.slideData.startPos = model_.worldTF.GetWorldPosition();

	//ベクトルをスライディング初速度で設定
	velo_ = Math::Vector3(pVeloX_, 0, 0).Normalize() * parameters_.slideData.spd;

	SetAnimation(A_SlidingStart, false);
	aabb_.aabb.min = slideAABB_.min;
	aabb_.aabb.max = slideAABB_.max;


	prePos_ = std::nullopt;
}

void Player::InitializeQuitSlide()
{
	SetAnimation(A_SlidingEnd, false);
	aabb_.aabb.min = standAABB_.min;
	aabb_.aabb.max = standAABB_.max;

}



void Player::InitializeJump()
{
	//初期加速度を渡す
	//acceGravity_ = jumpVelo_;
	Math::Vector3 velo;
	velo = Math::Vector3{ pVeloX_,0,0 }.Normalize();
	velo.y = parameters_.jumpData.jumpSlope_;
	velo = velo.Normalize();

	//慣性処理が消え去った
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
	parameters_.bulletData.currentPutBulletInSec_ = 0;
	parameters_.bulletData.currentReloadStartSec_ = 0;

	//SetAnimation(A_Idle);
	aabb_.aabb.min = standAABB_.min;
	aabb_.aabb.max = standAABB_.max;

	// ジャンプで射撃
	audioShot_.Play();
	// パーティクル生成
	Math::Vector3 p = model_.worldTF.GetWorldPosition();
	p.x += Math::Vector3{ pVeloX_,0,0 }.Normalize().x;
	particle_->Jump(p);
}
void Player::InitializeSlideStopShot()
{
	//向き方向と逆方向に吹っ飛ぶ
	Math::Vector3 velo;
	velo = Math::Vector3{ -pVeloX_,0,0 }.Normalize();
	velo.y = parameters_.slideData.jumpSlope_;
	velo = velo.Normalize();

	velo_ = velo * parameters_.slideData.startVelo;
	acce_.y = -parameters_.gravity;

	parameters_.jumpData.isJump_ = true;
	parameters_.bulletData.currentPutBulletInSec_ = 0;
	parameters_.bulletData.currentReloadStartSec_ = 0;

	SetAnimation(A_CutBack,false);
	aabb_.aabb.min = standAABB_.min;
	aabb_.aabb.max = standAABB_.max;


}
void Player::InitializeHitSomeone()
{
	Math::Vector3 ve = Math::Vector3{ parameters_.hitData.hitDirection_,0,0 }.Normalize();
	ve.y = parameters_.hitData.hitHeight_;

	velo_ = ve * parameters_.hitData.hitVelocity_;
	acce_.y = -parameters_.gravity;

	parameters_.hitData.currentNoHit_ = 0;
	parameters_.hitData.animeCount_ = 0;


	SetAnimation(A_Damage, false);

	//多分上に吹っ飛ぶので一応
	parameters_.jumpData.isJump_ = true;
	model_.isActive = false;


}

void Player::InitializeDead()
{
	parameters_.activeFlag.isActive = false;
	SetAnimation(A_Dead, false);

	//向きベクトル計算
	Math::Vector3 ve = Math::Vector3{ parameters_.hitData.hitDirection_,0,0 }.Normalize();
	ve.y = parameters_.deadData.slope;
	ve = ve.Normalize();

	velo_ = ve * parameters_.deadData.startVelo;
	acce_.y = -parameters_.gravity;

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
	if (LWP::Input::Keyboard::GetPress(DIK_A)|| Input::Keyboard::GetPress(DIK_LEFT)) {
		move.x -= 1;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_D)|| Input::Keyboard::GetPress(DIK_RIGHT)) {
		move.x += 1;
	}

	//パッドのX入力のみ反映
	move.x += Input::Pad::GetLStick().x;
	move = move.Normalize();
	move *= parameters_.moveSpd;

	//プレイヤーの向きを保存
	if (move.x != 0) {
		pVeloX_ = move.x;

		parameters_.currentInertia += delta;
	}
	else {
		parameters_.currentInertia -= delta * parameters_.stopDecelerationDouble_;
	}

	//たち射撃の場合さらにカウント減少
	if (nowPlayAnimeName_ == animeName_[A_StandShot]) {
		parameters_.currentInertia -= delta * parameters_.standShotDecelerationDouble_;
	}

	//0から最大値に値を合わせる
	if (parameters_.currentInertia < 0) {
		parameters_.currentInertia = 0;
	}
	else if (parameters_.currentInertia > parameters_.movingInertiaSec) {
		parameters_.currentInertia = parameters_.movingInertiaSec;
	}

	//0~1に変更
	float t = parameters_.currentInertia / parameters_.movingInertiaSec;

	//WorldTFに値追加
	model_.worldTF.translation += (move * t) * delta;

#pragma region アニメーション変更処理

	//弾の発射モーション以外は着地モーション優先
	if (nowPlayAnimeName_ != animeName_[A_Land] && nowPlayAnimeName_ != animeName_[A_Recovery]&& nowPlayAnimeName_ != animeName_[A_CutBack]) {

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
		if (nowPlayAnimeName_ != animeName_[A_Recovery]) {
			ToSliding();

			ToShot(Math::Vector3{ pVeloX_,0,0 }.Normalize(), standShot);
		}
		ToJump();
	}
#pragma endregion
}
void Player::UpdateSlide()
{
	//もし座標変更が行われていなかった場合
	if (prePos_ == model_.worldTF.translation) {
		//状態を変更して状態遷移
		behaviorReq_ = QuitSlide;
		return;
	}
	prePos_ = model_.worldTF.translation;

	//過去位置と現在（1F前)との距離を検索
	float pLeng = (parameters_.slideData.startPos.x - model_.worldTF.GetWorldPosition().x);
	if (pLeng < 0) {
		pLeng *= -1;
	}

	//指定した距離より離れた場合に減速処理
	if (parameters_.slideData.length < pLeng) {
		behaviorReq_ = QuitSlide;
	}

	//スライディング中のアニメーション切り替え
	if (animeName_[A_SlidingStart] == nowPlayAnimeName_ && !animation.GetPlaying()) {
		SetAnimation(A_Sliding);
	}

	//スライディング中の発砲処理
	float x = 0;
	if (Input::Keyboard::GetPress(DIK_D)|| Input::Keyboard::GetPress(DIK_RIGHT)) {
		x += 1;
	}
	if (Input::Keyboard::GetPress(DIK_A)|| Input::Keyboard::GetPress(DIK_LEFT)) {
		x -= 1;
	}
	x += Input::Pad::GetLStick().x;

	//向きが同じ場合特になし
	if ((x > 0 && pVeloX_ < 0) || (x < 0 && pVeloX_ > 0)) {

		if (pVeloX_ > 0) {
			x = 1;
		}
		else {
			x = -1;
		}

	}
	else {
		x = 0;
	}
	//スライド中に攻撃
	std::string type;
	bool ans = false;
	//水平射撃か
	if (x != 0) {
		//逆噴射処理
		if (parameters_.activeFlag.slidingStopShot) {
			type = standShot;
			ans = ShotBullet(Math::Vector3{ x,parameters_.slideData.shotSlope,0 }.Normalize(), type, (float)parameters_.bulletData.shotpelletNum_);
		}
		else {
			//通常処理
			if (Input::Keyboard::GetTrigger(DIK_SPACE) || Input::Pad::GetTrigger(XBOX_RT)) {
				type = slideShot;
				ans = ShotBullet(Math::Vector3{ 0,1,0 }.Normalize(), type, (float)parameters_.bulletData.shotpelletNum_);
			}
		}
	}
	else {
		if (Input::Keyboard::GetTrigger(DIK_SPACE) || Input::Pad::GetTrigger(XBOX_RT)) {
			type = slideShot;
			ans = ShotBullet(Math::Vector3{ 0,1,0 }.Normalize(), type, (float)parameters_.bulletData.shotpelletNum_);
		}
	}





	if (ans) {
		//状態に
		if (type == standShot) {
			SetAnimation(A_StandShot, false);
		}
		else {
			SetAnimation(A_SlidingShot, false);
		}

		//0じゃないとき処理
		if (x != 0 && parameters_.activeFlag.slidingStopShot) {
			behaviorReq_ = SlideStopShot;
		}
	}




	ToJump();
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
	//ジャンプ開始処理と
	if (nowPlayAnimeName_ == animeName_[A_JumpStart] && !animation.GetPlaying()) {
		SetAnimation(A_Jumping, true);
	}


	if (!parameters_.jumpData.isJump_) {
		behaviorReq_ = Moving;
		SetAnimation(A_Land, false);
	}
}

void Player::UpdateSlideStopShot()
{
	//着地で歩く処理に移行
	if (!parameters_.jumpData.isJump_) {
		behaviorReq_ = Moving;
	}

}

void Player::UpdateHitSomeone()
{

	//着地で移動処理
	if (!parameters_.jumpData.isJump_) {
		SetAnimation(A_Recovery, false);
		behaviorReq_ = Moving;
	}

}

void Player::UpdateDead()
{
}




#pragma endregion


