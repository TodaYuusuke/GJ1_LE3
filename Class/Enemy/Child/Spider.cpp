#include "Spider.h"

using namespace LWP;
using namespace LWP::Object;

void Spider::ChildInit() {
	model_.LoadShortPath("Enemy/SpiderEnemy/SpiderEnemy.gltf");
	animation_.LoadFullPath("resources/model/Enemy/SpiderEnemy/SpiderEnemy.gltf", &model_);
	// コライダー設定
	collider_.worldTF.translation.y = 1.0f;
	Collider::AABB& aabb = collider_.SetBroadShape(Collider::AABB());
	aabb.min.y = -1.0f;
	aabb.max.y = 1.0f;
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
