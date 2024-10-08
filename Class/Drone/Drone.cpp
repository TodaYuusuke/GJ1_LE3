#include "Drone.h"


using namespace LWP;
using namespace LWP::Math;

Drone::~Drone() {
	if (suction_.enemy != nullptr) {
		delete suction_.enemy;
	}
}

void Drone::Initialize(Player* playerPtr, EnemyManager* enemyPtr) {
	player_ = playerPtr;
	enemies_ = enemyPtr;

	model_.LoadShortPath("Drone/Drone.gltf");
	model_.materials["Gage"].enableLighting = false;
	animation_.LoadFullPath("resources/model/Drone/Drone.gltf", &model_);
	animation_.Play("00_Idle", true);

	light_.transform.Parent(&model_.worldTF);
	light_.transform.translation.y = -0.72f;

	light_.intensity = 0.5f;
	light_.radius = 10.0f;
	light_.color = { 202,255,208,255 };
}

void Drone::Update() {
#if DEMO
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Drone")) {
			ImGui::Text(("State : " + behaviorStirng_[behavior_]).c_str());
			if (ImGui::TreeNode("Model")) {
				model_.DebugGUI();
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Light")) {
				light_.DebugGUI();
				ImGui::TreePop();
			}
			ImGui::Checkbox("isActive", &isActive_);
			ImGui::Text("----- Parameter -----");
			ImGui::DragFloat("kSlerpT", &kSlerpT_, 0.01f);
			if (ImGui::TreeNode("UpgradeParameter")) {
				ImGui::Text("SuctionedDeadBody : %d", suctionedDeadBody_);
				ImGui::InputInt("kNeedDeadBody", &upgradeParamater.kNeedDeadBody);
				ImGui::DragFloat("kSuctionNeedTime", &upgradeParamater.kSuctionNeedTime, 0.01f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PlayerFollow")) {
				ImGui::DragFloat3("kOffset", &playerFollow_.kOffset.x, 0.01f);
				ImGui::DragFloat("kSearchRange", &playerFollow_.kSearchRange, 0.01f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("MoveDeadBody")) {
				ImGui::DragFloat("kRange", &moveDeadBody_.kRange, 0.01f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Suction")) {
				if (suction_.enemy && ImGui::TreeNode("Enemy")) {
					suction_.enemy->DebugGUI();
					ImGui::TreePop();
				}
				ImGui::DragFloat3("WorldPos", &suction_.worldPos.x, 0.01f);
				ImGui::DragFloat3("Scale", &suction_.scale.x, 0.01f);
				ImGui::DragFloat("time", &suction_.time, 0.01f);
				ImGui::TreePop();
			}
			ImGui::Text("----- State Change -----");
			if (ImGui::Button("PlayerFollow")) { behaviorReq_ = PlayerFollow; }
			if (ImGui::Button("MoveDeadBody")) { behaviorReq_ = MoveDeadBody; }
			if (ImGui::Button("Suction")) { behaviorReq_ = Behavior::Suction; }
			if (ImGui::Button("GenerateItem")) { behaviorReq_ = GenerateItem; }
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
#endif

	// 早期リターン
	if (!isActive_) { return; }

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

	// 地形より上に行かないように
	if (goalPosition_.y > 5.5f) { goalPosition_.y = 5.5f; }
	// モデルの座標更新
	model_.worldTF.translation = Utility::Interp::Lerp(model_.worldTF.translation, goalPosition_, kSlerpT_);

	// エリア外にいけないように
	if (model_.worldTF.translation.x < -outArea_) {
		model_.worldTF.translation.x = -outArea_;
	}
	else if (model_.worldTF.translation.x > outArea_) {
		model_.worldTF.translation.x = outArea_;
	}

	// アイテム生成
	if (suctionedDeadBody_ >= upgradeParamater.kNeedDeadBody) {
		heals_.emplace_back();
		heals_.back().Init(model_.worldTF.GetWorldPosition());
		suctionedDeadBody_ = 0;
	}
	// 吸収数に応じてマテリアル調整
	float t = float(suctionedDeadBody_) / float(upgradeParamater.kNeedDeadBody);
	model_.materials["Gage"].uvTransform.translation.y = (0.0f * (1.0f - t) - 0.5f * t);

	// 回復アイテム更新
	for (HealItem& h : heals_) {
		h.Update();
	}
	heals_.remove_if([](HealItem& h) {
		return h.GetUsed();
	});

}

//初期化関数ポンタテーブル
void (Drone::* Drone::BehaviorInitialize[])() = {
	&Drone::InitPlayerFollow,
	&Drone::InitMoveDeadBody,
	&Drone::InitSuction,
	&Drone::InitGenerateItem
};
//更新初期化関数ポインタテーブル
void (Drone::* Drone::BehaviorUpdate[])() = {
	&Drone::UpdatePlayerFollow,
	&Drone::UpdateMoveDeadBody,
	&Drone::UpdateSuction,
	&Drone::UpdateGenerateItem
};

void Drone::InitPlayerFollow() {}
void Drone::UpdatePlayerFollow() {
	// プレイヤーの上を飛ぶ
	Vector3 offset = playerFollow_.kOffset;
	offset.x *= player_->GetPlayerDirection();
	goalPosition_ = player_->GetWorldPosition() + offset;

	// プレイヤーがジャンプしても上に貫通しないように
	if (goalPosition_.y > 4.0f) {
		goalPosition_.y = 4.0f;
	}

	// 近くに死体があるかチェック
	IEnemy* e = enemies_->GetNearDeadBody(model_.worldTF.GetWorldPosition());
	if (e != nullptr) {
		behaviorReq_ = MoveDeadBody;	// 死体の位置に移動
		suction_.enemy = e;
	}
}
void Drone::InitMoveDeadBody() {
	// 死体の近くにいくまで移動
	Vector3 bodyPos = suction_.enemy->GetWorldPosition();
	goalPosition_ = bodyPos + ((model_.worldTF.GetWorldPosition() - bodyPos).Normalize() * moveDeadBody_.kRange);
}
void Drone::UpdateMoveDeadBody() {
	// 近くについたら吸収
	if (Vector3::Distance(model_.worldTF.GetWorldPosition(), goalPosition_) <= 0.05f) {
		behaviorReq_ = Suction;
	}
}
void Drone::InitSuction() {
	// エネミーが空ならプレイヤー追従に戻る
	if (suction_.enemy == nullptr) {
		behaviorReq_ = PlayerFollow;
	}
	else {
		suction_.worldPos = suction_.enemy->GetWorldPosition();	// 死体の場所
		suction_.scale = suction_.enemy->model_.worldTF.scale;	// 死体のスケール
	}
	// タイマーリセット
	suction_.time = 0.0f;
}
void Drone::UpdateSuction() {
	// 早期リターン
	if (suction_.enemy == nullptr) { return; }

	// 経過時間を加算
	suction_.time += LWP::Info::GetDeltaTimeF();
	// 経過時間を過ぎたら吸収完了
	if (suction_.time > upgradeParamater.kSuctionNeedTime) {
		delete suction_.enemy;
		suction_.enemy = nullptr;
		suctionedDeadBody_++;	// 吸収数+1

		behaviorReq_ = PlayerFollow;	// プレイヤー追従に戻る
	}
	else{
		// 吸収しているようにアニメーション
		Vector3 pos = model_.worldTF.GetWorldPosition();
		pos.y += 0.7f;
		suction_.enemy->model_.worldTF.translation = Utility::Interp::Lerp(suction_.worldPos, pos, suction_.time / upgradeParamater.kSuctionNeedTime);
		suction_.enemy->model_.worldTF.scale = Utility::Interp::Slerp(suction_.scale, { 0.0f,0.0f,0.0f }, suction_.time / upgradeParamater.kSuctionNeedTime);
	}
}
void Drone::InitGenerateItem() {

}
void Drone::UpdateGenerateItem() {

}

