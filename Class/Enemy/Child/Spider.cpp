#include "Spider.h"

using namespace LWP;
using namespace LWP::Object;

void Spider::ChildInit() {
	model_.LoadShortPath("Enemy/SpiderEnemy/SpiderEnemy.gltf");
	animation_.LoadFullPath("resources/model/Enemy/SpiderEnemy/SpiderEnemy.gltf", &model_);
	animation_.name = "Spider";
	// コライダー設定
	collider_.worldTF.translation.y = 1.0f;
	collider_.name = "Spider";
	Collider::AABB& aabb = collider_.SetBroadShape(Collider::AABB());
	aabb.min.y = -1.0f;
	aabb.max.y = 1.0f;
}

void Spider::ChildUpdate() {}

void Spider::DebugGUI() {
	// 状態
	ImGui::Text(("State : " + behaviorStirng_[behavior_]).c_str());
	if (ImGui::TreeNode("Model")) {
		model_.DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Animation")) {
		animation_.DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Collider")) {
		collider_.DebugGUI();
		ImGui::TreePop();
	}
	ImGui::Checkbox("isAlive:", &isAlive_);
	ImGui::Text("----- State Change -----");
	if (ImGui::Button("Normal")) { behaviorReq_ = Normal; }
	if (ImGui::Button("Knockback")) { behaviorReq_ = Knockback; }
	if (ImGui::Button("Dying")) { behaviorReq_ = Dying; }
	if (ImGui::Button("DeadBody")) { behaviorReq_ = DeadBody; }
}

void Spider::InitNormal() {
	// アニメーション再生
	animation_.Play("01_Walk", true);
}
void Spider::UpdateNormal() {
	float delta = Info::GetDefaultDeltaTimeF();

	//プレイヤー方向への向きベクトルを計算
	Math::Vector3 velo = (player_->GetWorldPosition() - model_.worldTF.GetWorldPosition()).Normalize();
	velo.y = 0;
	//移動速度をかけた分移動
	model_.worldTF.translation += velo * (spd_ * delta);
}

void Spider::InitKnockback() {
	IEnemy::InitKnockback();
	// アニメーション再生
	animation_.Play("00_Idle", true);
}
void Spider::InitDying() {
	IEnemy::InitDying();
	// アニメーション再生
	animation_.Play("02_Die");
}