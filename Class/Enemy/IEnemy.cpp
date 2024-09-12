#include "IEnemy.h"
#include "../Mask/Mask.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;

void IEnemy::Init() {
	// コライダーの基本設定
	collider_.SetFollowTarget(&model_.worldTF);
	collider_.name = "Enemy";
	collider_.enterLambda = [this](Collider::Collider* hitTarget) {
		// 無敵時間中ならば戻る
		if (invincibleTime_ > 0.0f) {
			return;
		}

		// プレイヤーの弾だった場合
		if (hitTarget->name == "StandBullet") {
			behaviorReq_ = Knockback;
			invincibleTime_ = kInvincibleTime_;
		}
		else if (hitTarget->name == "SlidingBullet") {
			SlidingHit();	// 被弾
			invincibleTime_ = kInvincibleTime_;
		}
	};
	collider_.mask.SetBelongFrag(GJMask::Enemy());	// フラグ設定
	collider_.mask.SetHitFrag(GJMask::Player() | GJMask::Bullet());
	
	// 子クラスの初期化
	ChildInit();
	// 状態の初期化
	InitNormal();
}

void IEnemy::Update() {
	//状態リクエストがある時実行
	if (behaviorReq_) {
		//過去の状態を保存
		preBehavior_ = behavior_;
		//状態を更新
		behavior_ = behaviorReq_.value();
		behaviorReq_ = std::nullopt;
		//初期化処理
		(this->*BehaviorInitialize[(int)behavior_])();
	}
	//状態更新処理
	(this->*BehaviorUpdate[behavior_])();

	// 子クラスのアップデート
	ChildUpdate();

	// 被弾無敵フレームを現象
	invincibleTime_ -= LWP::Info::GetDeltaTimeF();
	if (invincibleTime_ < 0.0f) {
		invincibleTime_ = 0.0f;
	}
}

void IEnemy::SlidingHit() {
	health_--;

	// 体力が0以下になったら死体に
	if (health_ <= 0) {
		behaviorReq_ = Dying;
		isAlive_ = false;	// 死亡
	}
	else {
		// 体力が減った演出をここに
	}
}

void IEnemy::DebugGUI() {
	// 状態
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
	ImGui::Text("----- State Change -----");
	if (ImGui::Button("Normal")) { behaviorReq_ = Normal; }
	if (ImGui::Button("Knockback")) { behaviorReq_ = Knockback; }
	if (ImGui::Button("Dying")) { behaviorReq_ = Dying; }
	if (ImGui::Button("DeadBody")) { behaviorReq_ = DeadBody; }
}

//初期化関数ポンタテーブル
void (IEnemy::* IEnemy::BehaviorInitialize[])() = {
	&IEnemy::InitNormal,
	&IEnemy::InitKnockback,
	&IEnemy::InitDying,
	&IEnemy::InitDeadBody
};
//更新初期化関数ポインタテーブル
void (IEnemy::* IEnemy::BehaviorUpdate[])() = {
	&IEnemy::UpdateNormal,
	&IEnemy::UpdateKnockback,
	&IEnemy::UpdateDying,
	&IEnemy::UpdateDeadBody
};

void IEnemy::InitKnockback() {
	knockback_.time = 0.0f;	// 時間初期化
	// 方向を決める
	float dir = model_.worldTF.translation.x - player_->GetWorldPosition().x;
	knockback_.dir = dir / std::sqrtf(dir * dir);	// 正規化
}
void IEnemy::UpdateKnockback() {
	// 経過時間更新
	float deltaTime = LWP::Info::GetDeltaTimeF();
	knockback_.time += deltaTime;

	// ノックバック時間を超えたら終了
	if (knockback_.time >= knockback_.kTotalTime) {
		deltaTime = knockback_.time - knockback_.kTotalTime;
		behaviorReq_ = Normal;
	}
	// 移動
	model_.worldTF.translation.x += (knockback_.kDistance / knockback_.kTotalTime) * knockback_.dir * deltaTime;
}
void IEnemy::InitDying() {
	// 速度は破棄
	velocity_ = { 0.0f,0.0f,0.0f };
}
void IEnemy::UpdateDying() {
	// 一定の速度で上昇
	model_.worldTF.translation.y += dying_.kUpSpeed;
	// 天井に着いたら停止
	if (model_.worldTF.translation.y >= dying_.kMaxY) {
		model_.worldTF.translation.y = dying_.kMaxY;
		behaviorReq_ = DeadBody;
	}
}
void IEnemy::InitDeadBody() {
}
void IEnemy::UpdateDeadBody() {
}