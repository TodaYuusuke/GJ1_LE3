#include "HealItem.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object::Collider;

void HealItem::Init(LWP::Math::Vector3 pos) {
	model_.LoadCube();
	model_.worldTF.translation = pos;
	model_.worldTF.scale = {0.3f,0.3f,0.3f};
	//model_.materials[0].color = Utility::ColorPattern::GREEN;

	AABB& aabb = collider_.SetBroadShape(AABB());
	aabb.min *= 0.5f;
	aabb.max *= 0.5f;
	collider_.SetFollowTarget(&model_.worldTF);
	collider_.name = "HealItem";
	collider_.enterLambda = [this](Collider* col) {
		if (col->name == "player") {
			isUsed_ = true;
		}
	};
}
void HealItem::Update() {
	ImGui::Begin("Test");
	ImGui::DragFloat("kRotationSpeed", &kRotationSpeed_, 0.01f);
	ImGui::DragFloat("kFallSpeed", &kFallSpeed_, 0.01f);
	ImGui::End();

	// アニメーション
	model_.worldTF.translation.y -= kFallSpeed_;
	if (model_.worldTF.translation.y < 0.0f) {
		model_.worldTF.translation.y = 0.0f;
	}
	model_.worldTF.rotation *= Quaternion::CreateFromAxisAngle(Vector3::UnitY(), kRotationSpeed_);
}
