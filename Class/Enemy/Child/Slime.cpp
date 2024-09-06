#include "Slime.h"

using namespace LWP;
using namespace LWP::Object;

void Slime::Initialize() {
	model_.LoadSphere();
	collider_.SetBroadShape(Collider::Sphere());
	collider_.SetFollowTarget(&model_.worldTF);
	collider_.name = "enemy";
}

void Slime::Update() {

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