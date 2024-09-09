#include "IEnemy.h"

void IEnemy::DeadUpdate() {
	// ����łȂ������瑁�����^�[��
	if (isAlive_) { return; }

	// ���̑��x�ŏ㏸
	model_.worldTF.translation.y += kUpSpeed_;
	// �V��ɒ��������~
	if (model_.worldTF.translation.y >= kMaxY_) {
		model_.worldTF.translation.y = kMaxY_;
	}
}

void IEnemy::Hit() {
	health_--;

	// �̗͂�0�ȉ��ɂȂ����玀�̂�
	if (health_ <= 0) {
		isAlive_ = false;	// ���S
	}
	else {
		// �̗͂����������o��������
	}
}

void IEnemy::DebugGUI() {
	if (ImGui::TreeNode("Model")) {
		model_.DebugGUI();
		ImGui::TreePop();
	}
	ImGui::Checkbox("isAlive:", &isAlive_);
}