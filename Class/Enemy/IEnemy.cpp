#include "IEnemy.h"

void IEnemy::DeadUpdate() {
	// 死んでなかったら早期リターン
	if (isAlive_) { return; }

	// 一定の速度で上昇
	model_.worldTF.translation.y += kUpSpeed_;
	// 天井に着いたら停止
	if (model_.worldTF.translation.y >= kMaxY_) {
		model_.worldTF.translation.y = kMaxY_;
	}
}

void IEnemy::Hit() {
	health_--;

	// 体力が0以下になったら死体に
	if (health_ <= 0) {
		isAlive_ = false;	// 死亡
	}
	else {
		// 体力が減った演出をここに
	}
}

void IEnemy::DebugGUI() {
	if (ImGui::TreeNode("Model")) {
		model_.DebugGUI();
		ImGui::TreePop();
	}
	ImGui::Checkbox("isAlive:", &isAlive_);
}