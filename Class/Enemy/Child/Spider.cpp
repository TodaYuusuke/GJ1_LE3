#include "Spider.h"

using namespace LWP;
using namespace LWP::Object;

void Spider::Initialize() {
	model_.LoadShortPath("Enemy/SpiderEnemy/SpiderEnemy.gltf");
	animation_.LoadFullPath("resources/model/Enemy/SpiderEnemy/SpiderEnemy.gltf", &model_);
	collider_.SetBroadShape(Collider::AABB());
	collider_.worldTF.translation.y = 2.0f;
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
	// normal以外ならばアニメーション停止
	if (behavior_ != Normal) {
		animation_.Stop();
	}
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
