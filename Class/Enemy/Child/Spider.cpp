#include "Spider.h"

using namespace LWP;
using namespace LWP::Object;

void Spider::Initialize() {
	model_.LoadShortPath("Enemy/Enemy.gltf");
	collider_.SetBroadShape(Collider::Sphere());
	collider_.SetFollowTarget(&model_.worldTF);
	collider_.name = "enemy";
	collider_.enterLambda = [this](Collider::Collider* hitTarget) {
		// プレイヤーの弾だった場合
		if (hitTarget->name == "playerBullet") {
			Hit();	// 被弾
		}
	};
}

void Spider::Update() {
	// 死んでたら早期リターン
	if (!isAlive_) { return; }

	float delta = Info::GetDefaultDeltaTimeF();

	//プレイヤー方向への向きベクトルを計算
	Math::Vector3 velo = (player_->GetWorldPosition() - model_.worldTF.GetWorldPosition()).Normalize();
	velo.y = 0;
	//移動速度をかけた分移動
	model_.worldTF.translation += velo * (spd_ * delta);
}