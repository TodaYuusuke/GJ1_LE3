#include "Drone.h"

using namespace LWP;
using namespace LWP::Math;

void Drone::Initialize(Player* playerPtr, EnemyManager* enemyPtr) {
	player_ = playerPtr;
	enemies_ = enemyPtr;

	model_.LoadShortPath("Drone/Drone.gltf");
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
			ImGui::Checkbox("isActive", &isActive);
			ImGui::Text("----- Parameter -----");
			ImGui::DragFloat("kSlerpT", &kSlerpT, 0.01f);
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
				ImGui::DragFloat("time", &suction_.time, 0.01f);
				ImGui::DragFloat("kTotalTime", &suction_.kTotalTime, 0.01f);
				ImGui::TreePop();
			}

			struct Suction {
				IEnemy* enemy = nullptr;	// 回収する死体のポインタ
				float time = 2.0f;	// 経過時間

				float kTotalTime = 2.0f;	// かかる時間
			}suction_;

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
	if (!isActive) { return; }

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

	// モデルの座標更新
	model_.worldTF.translation = Utility::Interp::Lerp(model_.worldTF.translation, goalPosition, kSlerpT);
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
	goalPosition = player_->GetWorldPosition() + offset;

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
	goalPosition = bodyPos + ((model_.worldTF.GetWorldPosition() - bodyPos).Normalize() * moveDeadBody_.kRange);
}
void Drone::UpdateMoveDeadBody() {
	// 近くについたら吸収
	if (Vector3::Distance(model_.worldTF.GetWorldPosition(), goalPosition) <= 0.05f) {
		behaviorReq_ = Suction;
	}
}
void Drone::InitSuction() {
	// エネミーが空ならプレイヤー追従に戻る
	if (suction_.enemy == nullptr) {
		behaviorReq_ = PlayerFollow;
	}
	suction_.time = 0.0f;
}
void Drone::UpdateSuction() {
	// 早期リターン
	if (suction_.enemy == nullptr) { return; }

	// 経過時間を加算
	suction_.time += LWP::Info::GetDeltaTimeF();
	// 経過時間を過ぎたら吸収完了
	if (suction_.time > suction_.kTotalTime) {
		behaviorReq_ = PlayerFollow;
		delete suction_.enemy;
	}
}
void Drone::InitGenerateItem() {

}
void Drone::UpdateGenerateItem() {

}

