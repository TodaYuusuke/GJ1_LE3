#include "Slime.h"

using namespace LWP;
using namespace LWP::Object;

void Slime::ChildInit() {
	model_.LoadShortPath("Enemy/SlimeEnemy/SlimeEnemy.gltf");
	// コライダー設定
	collider_.worldTF.translation.y = 1.0f;
	collider_.name = "Slime";
	Collider::Sphere& sphere = collider_.SetBroadShape(Collider::Sphere());
	sphere.position.y = -0.18f;
	sphere.radius = 0.51f;

	float s = 0.5f;
	model_.worldTF.scale = { s,s,s };
}

void Slime::ChildUpdate() {
	// 死んだら更新はしない
	if (!isAlive_) { return; }

	normal_.velocity.y += normal_.kGravity;
	model_.worldTF.translation += normal_.velocity;

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
	if (preBehavior_ == Knockback) {
		// 半分の時間でジャンプ
		normal_.jumpInterval = normal_.kJumpIntervalTime / 2.0f;
	}
	else {
		normal_.jumpInterval = normal_.kJumpIntervalTime;
	}
}
void Slime::UpdateNormal() {
	// 一定以下の高度になったらvelocityのxを0に
	if (model_.worldTF.translation.y <= 0.05f) {
		normal_.velocity.x = 0.0f;
	}

	// インターバル減少
	normal_.jumpInterval -= Info::GetDefaultDeltaTimeF();
	// 0を切ったらジャンプ
	if (normal_.jumpInterval < 0.0f) {
		// インターバルリセット
		normal_.jumpInterval = normal_.kJumpIntervalTime;

		// 方向を決める
		float dir = model_.worldTF.translation.x - player_->GetWorldPosition().x;
		dir = dir / std::sqrtf(dir * dir);	// 正規化
		// 速度加算
		normal_.velocity = normal_.kJumpVelocity;
		normal_.velocity.x *= dir;
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