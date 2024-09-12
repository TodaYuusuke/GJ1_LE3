#include "HealItem.h"
#include "../Mask/Mask.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object::Collider;

void HealItem::Init(LWP::Math::Vector3 pos) {
	model_.LoadShortPath("Drone/HealItem/HealItem.gltf");
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
	collider_.mask.SetBelongFrag(GJMask::HealItem());	// フラグ設定
	collider_.mask.SetHitFrag(GJMask::Player());
}
void HealItem::Update() {
	// アニメーション
	model_.worldTF.translation.y -= kFallSpeed_;
	if (model_.worldTF.translation.y < 0.0f) {
		model_.worldTF.translation.y = 0.0f;
	}
	model_.worldTF.rotation *= Quaternion::CreateFromAxisAngle(Vector3::UnitY(), kRotationSpeed_);
}
