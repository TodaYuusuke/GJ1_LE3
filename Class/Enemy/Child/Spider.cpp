#include "Spider.h"

using namespace LWP;
using namespace LWP::Object;

void Spider::Initialize() {
	model_.LoadShortPath("Enemy/Enemy.gltf");
	collider_.SetBroadShape(Collider::AABB());
	collider_.worldTF.translation.y = 1.0f;
	collider_.SetFollowTarget(&model_.worldTF);
	collider_.name = "Enemy";
	collider_.enterLambda = [this](Collider::Collider* hitTarget) {
		// プレイヤーの弾だった場合
		if (hitTarget->name == "StandBullet") {
			behaviorReq_ = Knockback;
		}
		else if (hitTarget->name == "SlidingBullet") {
			SlidingHit();	// 被弾
		}
	};
}

void Spider::ChildUpdate() {
	
}

void Spider::InitNormal() {

}
void Spider::UpdateNormal() {
	float delta = Info::GetDefaultDeltaTimeF();

	//プレイヤー方向への向きベクトルを計算
	Math::Vector3 velo = (player_->GetWorldPosition() - model_.worldTF.GetWorldPosition()).Normalize();
	velo.y = 0;
	//移動速度をかけた分移動
	model_.worldTF.translation += velo * (spd_ * delta);
}
