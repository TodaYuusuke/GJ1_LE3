#include "IEnemy.h"

using namespace LWP::Math;

void IEnemy::Update() {
	//��ԃ��N�G�X�g�����鎞���s
	if (behaviorReq_) {
		//�ߋ��̏�Ԃ�ۑ�
		preBehavior_ = behavior_;
		//��Ԃ��X�V
		behavior_ = behaviorReq_.value();
		behaviorReq_ = std::nullopt;
		//����������
		(this->*BehaviorInitialize[(int)behavior_])();
	}
	//��ԍX�V����
	(this->*BehaviorUpdate[behavior_])();

	// �q�N���X�̃A�b�v�f�[�g
	ChildUpdate();
}

void IEnemy::SlidingHit() {
	health_--;

	// �̗͂�0�ȉ��ɂȂ����玀�̂�
	if (health_ <= 0) {
		behaviorReq_ = Dying;
		isAlive_ = false;	// ���S
	}
	else {
		// �̗͂����������o��������
	}
}

void IEnemy::DebugGUI() {
	// ���
	ImGui::Text(("State : " + behaviorStirng_[behavior_]).c_str());
	if (ImGui::TreeNode("Model")) {
		model_.DebugGUI();
		ImGui::TreePop();
	}
	ImGui::Checkbox("isAlive:", &isAlive_);
	ImGui::Text("----- State Change -----");
	if (ImGui::Button("Normal")) { behaviorReq_ = Normal; }
	if (ImGui::Button("Knockback")) { behaviorReq_ = Knockback; }
	if (ImGui::Button("Dying")) { behaviorReq_ = Dying; }
	if (ImGui::Button("DeadBody")) { behaviorReq_ = DeadBody; }
}

//�������֐��|���^�e�[�u��
void (IEnemy::* IEnemy::BehaviorInitialize[])() = {
	&IEnemy::InitNormal,
	&IEnemy::InitKnockback,
	&IEnemy::InitDying,
	&IEnemy::InitDeadBody
};
//�X�V�������֐��|�C���^�e�[�u��
void (IEnemy::* IEnemy::BehaviorUpdate[])() = {
	&IEnemy::UpdateNormal,
	&IEnemy::UpdateKnockback,
	&IEnemy::UpdateDying,
	&IEnemy::UpdateDeadBody
};

void IEnemy::InitKnockback() {
	knockback_.time = 0.0f;	// ���ԏ�����
	// ���������߂�
	float dir = model_.worldTF.translation.x - player_->GetWorldPosition().x;
	knockback_.dir = dir / std::sqrtf(dir * dir);	// ���K��
}
void IEnemy::UpdateKnockback() {
	// �o�ߎ��ԍX�V
	float deltaTime = LWP::Info::GetDeltaTimeF();
	knockback_.time += deltaTime;

	// �m�b�N�o�b�N���Ԃ𒴂�����I��
	if (knockback_.time >= knockback_.kTotalTime) {
		deltaTime = knockback_.time - knockback_.kTotalTime;
		behaviorReq_ = Normal;
	}
	// �ړ�
	model_.worldTF.translation.x += (knockback_.kDistance / knockback_.kTotalTime) * knockback_.dir * deltaTime;
}
void IEnemy::InitDying() {
	// ���x�͔j��
	velocity_ = { 0.0f,0.0f,0.0f };
}
void IEnemy::UpdateDying() {
	// ���̑��x�ŏ㏸
	model_.worldTF.translation.y += dying_.kUpSpeed;
	// �V��ɒ��������~
	if (model_.worldTF.translation.y >= dying_.kMaxY) {
		model_.worldTF.translation.y = dying_.kMaxY;
		behaviorReq_ = DeadBody;
	}
}
void IEnemy::InitDeadBody() {

}
void IEnemy::UpdateDeadBody() {

}