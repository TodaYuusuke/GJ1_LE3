#include "TestScene.h"

// 初期化
void TestScene::Initialize() {
	enemyManager_.Initialize(&player_);
}

// 更新
void TestScene::Update() {
#if DEMO
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Player")) {
			ImGui::DragFloat3("position", &player_.position.x, 0.01f);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
#endif

	enemyManager_.Update();
}