#include "Slime.h"

using namespace LWP;
using namespace LWP::Object;

void Slime::ChildInit() {
	model_.LoadShortPath("Enemy/SpiderEnemy/SpiderEnemy.gltf");
	//model_.LoadShortPath("Enemy/SlimeEnemy/SlimeEnemy.gltf");
	// �R���C�_�[�ݒ�
	collider_.worldTF.translation.y = 1.0f;
	Collider::AABB& aabb = collider_.SetBroadShape(Collider::AABB());
	aabb.min.y = -1.0f;
	aabb.max.y = 1.0f;
}

void Slime::ChildUpdate() {
	// ���񂾂�X�V�͂��Ȃ�
	if (!isAlive_) { return; }

	normal_.velocity.y += normal_.kGravity;
	model_.worldTF.translation += normal_.velocity;

	// ���ȉ��̍��x�ɂȂ�����velocity��x��0��
	if (model_.worldTF.translation.y <= 0.5f) {
		normal_.velocity.x = 0.0f;
	}
	// �X���C�����n�ʂɖ��܂�Ȃ��悤��
	if (model_.worldTF.translation.y < 0.0f) {
		model_.worldTF.translation.y = 0.0f;
	}
}

void Slime::DebugGUI() {
	// ���
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
	// �C���^�[�o�����Z�b�g
	normal_.jumpInterval = normal_.kJumpIntervalTime;
}
void Slime::UpdateNormal() {

	// �C���^�[�o������
	normal_.jumpInterval -= Info::GetDefaultDeltaTimeF();
	// 0��؂�����W�����v
	if (normal_.jumpInterval < 0.0f) {
		// �C���^�[�o�����Z�b�g
		normal_.jumpInterval = normal_.kJumpIntervalTime;

		// ���������߂�
		float dir = model_.worldTF.translation.x - player_->GetWorldPosition().x;
		dir;	// �܂�����Ȃ�
		// ���x���Z
		normal_.velocity = normal_.kJumpVelocity;
	}
}
void Slime::InitKnockback() {
	knockback_.time = 0.0f;	// ���ԏ�����
	// ���������߂�
	float dir = model_.worldTF.translation.x - player_->GetWorldPosition().x;
	knockback_.dir = dir / std::sqrtf(dir * dir);	// ���K��

	// �W�����v���������ꍇ���������邽��velocity��y��0��
	normal_.velocity.y = 0.0f;
}
/*
void Slime::Update() {
	// ����ł��瑁�����^�[��
	if (!isAlive_) { return; }

	float delta = LWP::Info::GetDefaultDeltaTimeF();

	//���̃W�����v�܂ł̃J�E���g
	if (!isJump_) {
		jumpCount_ += delta;
	}

	//�J�E���g���ŃW�����v
	if (jumpCount_ >= maxjumpTime_) {
		jumpCount_ -= maxjumpTime_;
		isJump_ = true;

		//�v���C���[�����ɃW�����v
		float direcX = player_->GetWorldPosition().x - model_.worldTF.GetWorldPosition().x;

		//�����x�N�g��
		LWP::Math::Vector3 velo;
		if (direcX > 0) {
			//�v���C���[��+����
			velo = LWP::Math::Vector3{ 1,jumpYNum_,0 }.Normalize();

		}
		else {
			//�v���C���[���|����
			velo = LWP::Math::Vector3{ -1,jumpYNum_,0 }.Normalize();
		}
		//�W�����v�������ăx�N�g���ɒǉ�
		velo *= jumpSpd_;
		velo_ = velo;
	}

	velo_.y += gravity_ * delta;

	//�ړ�����
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