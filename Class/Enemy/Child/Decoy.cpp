#include "Decoy.h"

using namespace LWP;
using namespace LWP::Object;

void Decoy::ChildInit() {
	model_.LoadShortPath("Enemy/SpiderEnemy/SpiderEnemy.gltf");
	animation_.LoadFullPath("resources/model/Enemy/SpiderEnemy/SpiderEnemy.gltf", &model_);
	animation_.name = "Decoy";
	// コライダー設定
	collider_.worldTF.translation.y = 1.0f;
	collider_.name = "Spider";
	Collider::AABB& aabb = collider_.SetBroadShape(Collider::AABB());
	aabb.min.y = -0.17f;
	aabb.max.y = 0.7f;

	audioDead_.Load(audioPath_ + deadPath_);
}

void Decoy::ChildUpdate() {}

void Decoy::DebugGUI() {}

void Decoy::InitNormal() {
	// アニメーション再生
	animation_.Play("00_Idle", true);
}
void Decoy::UpdateNormal() {}
