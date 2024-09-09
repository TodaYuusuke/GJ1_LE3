#include "Slime.h"

using namespace LWP;
using namespace LWP::Object;

void Slime::ChildInit() {
	model_.LoadShortPath("Enemy/SpiderEnemy/SpiderEnemy.gltf");
	//model_.LoadShortPath("Enemy/SlimeEnemy/SlimeEnemy.gltf");
	// コライダー設定
	collider_.worldTF.translation.y = 1.0f;
	Collider::AABB& aabb = collider_.SetBroadShape(Collider::AABB());
	aabb.min.y = -1.0f;
	aabb.max.y = 1.0f;
}

void Slime::ChildUpdate() {
	// 死んだら更新はしない
	if (!isAlive_) { return; }

	normal_.velocity.y += normal_.kGravity;
	model_.worldTF.translation += normal_.velocity;

	// 一定以下の高度になったらvelocityのxを0に
	if (model_.worldTF.translation.y <= 0.5f) {
		normal_.velocity.x = 0.0f;
	}
	// スライムが地面に埋まらないように
	if (model_.worldTF.translation.y < 0.0f) {
		model_.worldTF.translation.y = 0.0f;
	}
}

void Slime::DebugGUI() {
	// 状態
	ImGui::Text(("State : " + behaviorStirng_[behavior_]).c_str());
	if (ImGui::TreeNode("Model")) {
		model_.DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Collider")) {
		collider_.DebugGUI();
		ImGui::TreePop();
	}
	ImGui::Checkbox("isAlive:", &isAlive_);
	ImGui::Text("----- Parameter -----");
	ImGui::DragFloat("kJumpIntervalTime", &normal_.kJumpIntervalTime, 0.01f);
	ImGui::DragFloat3("kJumpVelocity", &normal_.kJumpVelocity.x, 0.01f);
	ImGui::DragFloat("kGravity", &normal_.kGravity, 0.01f);
	ImGui::Text("----- State Change -----");
	if (ImGui::Button("Normal")) { behaviorReq_ = Behavior::Normal; }
	if (ImGui::Button("Knockback")) { behaviorReq_ = Knockback; }
	if (ImGui::Button("Dying")) { behaviorReq_ = Dying; }
	if (ImGui::Button("DeadBody")) { behaviorReq_ = DeadBody; }
}

void Slime::InitNormal() {
	// インターバルリセット
	normal_.jumpInterval = normal_.kJumpIntervalTime;
}
void Slime::UpdateNormal() {

	// インターバル減少
	normal_.jumpInterval -= Info::GetDefaultDeltaTimeF();
	// 0を切ったらジャンプ
	if (normal_.jumpInterval < 0.0f) {
		// インターバルリセット
		normal_.jumpInterval = normal_.kJumpIntervalTime;

		// 方向を決める
		float dir = model_.worldTF.translation.x - player_->GetWorldPosition().x;
		dir;	// まだつかわない
		// 速度加算
		normal_.velocity = normal_.kJumpVelocity;
	}
}
void Slime::InitKnockback() {
	knockback_.time = 0.0f;	// 時間初期化
	// 方向を決める
	float dir = model_.worldTF.translation.x - player_->GetWorldPosition().x;
	knockback_.dir = dir / std::sqrtf(dir * dir);	// 正規化

	// ジャンプ中だった場合落下させるためvelocityのyを0に
	normal_.velocity.y = 0.0f;
}
/*
void Slime::Update() {
	// 死んでたら早期リターン
	if (!isAlive_) { return; }

	float delta = LWP::Info::GetDefaultDeltaTimeF();

	//次のジャンプまでのカウント
	if (!isJump_) {
		jumpCount_ += delta;
	}

	//カウント一定でジャンプ
	if (jumpCount_ >= maxjumpTime_) {
		jumpCount_ -= maxjumpTime_;
		isJump_ = true;

		//プレイヤー方向にジャンプ
		float direcX = player_->GetWorldPosition().x - model_.worldTF.GetWorldPosition().x;

		//向きベクトル
		LWP::Math::Vector3 velo;
		if (direcX > 0) {
			//プレイヤーが+方向
			velo = LWP::Math::Vector3{ 1,jumpYNum_,0 }.Normalize();

		}
		else {
			//プレイヤーが－方向
			velo = LWP::Math::Vector3{ -1,jumpYNum_,0 }.Normalize();
		}
		//ジャンプ分かけてベクトルに追加
		velo *= jumpSpd_;
		velo_ = velo;
	}

	velo_.y += gravity_ * delta;

	//移動処理
	model_.worldTF.translation += velo_ * delta;

	if (model_.worldTF.translation.y < 0) {
		model_.worldTF.translation.y = 0;
		isJump_ = false;
		velo_ = { 0,0,0 };
	}
}
void Slime::DebugGUI() {

	bool isJ = false;

	if (ImGui::TreeNode("Model")) {
		model_.DebugGUI();
		ImGui::TreePop();
	}
	ImGui::Checkbox("isAlive:", &isAlive_);

	ImGui::Text("count : %4.1f", jumpCount_);
	ImGui::DragFloat("jump time sec", &maxjumpTime_);
	ImGui::Checkbox("Jump", &isJ);
	ImGui::DragFloat("Jump Y velo", &jumpYNum_);
	ImGui::DragFloat("Jump initial v/sec", &jumpSpd_);
	ImGui::DragFloat("gravity", &gravity_);

	if (isJ) {
		jumpCount_ += maxjumpTime_;
	}
}
*/